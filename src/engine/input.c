#include <stdio.h>
#include <string.h>
#include <time.h>
#include "keyboard.h"
#include "engine/input.h"

// protocolo de teclado do kitty: 1 (desambigua) + 2 (eventos de repetir/soltar) + 8 (todas as teclas como CSI)
#define KITTY_FLAGS 11
#define DETECT_TIMEOUT_S 0.3
#define INPUT_BUF_SIZE 256

#define EVENT_PRESS   1
#define EVENT_REPEAT  2
#define EVENT_RELEASE 3

#define MOD_CTRL 4

static int initialized = 0;
static int kitty = 0;
static int down[INPUT_KEY_COUNT];
static int prev_down[INPUT_KEY_COUNT];

// modo kitty: estado real da tecla + "apertou neste frame" (um toque curto dura ao menos 1 frame)
static int held[INPUT_KEY_COUNT];
static int tapped[INPUT_KEY_COUNT];

// modo reserva (terminal sem kitty): tecla segurada = chegou nos ultimos INPUT_HOLD_MS
static double last_seen[INPUT_KEY_COUNT];

static unsigned char buf[INPUT_BUF_SIZE];
static size_t buf_len = 0;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static int map_char(int ch) {
    switch(ch) {
        case 'w': case 'W': return INPUT_KEY_W;
        case 'a': case 'A': return INPUT_KEY_A;
        case 's': case 'S': return INPUT_KEY_S;
        case 'd': case 'D': return INPUT_KEY_D;
        case ' ':  return INPUT_KEY_SPACE;
        case '\n': case '\r': return INPUT_KEY_ENTER;
        case 27:   return INPUT_KEY_ESC_QUIT;
        case 3:    return INPUT_KEY_ESC_QUIT; // Ctrl+C: a cli-lib desliga o SIGINT
        default:   return -1;
    }
}

static int map_arrow(int final) {
    switch(final) {
        case 'A': return INPUT_KEY_UP;
        case 'B': return INPUT_KEY_DOWN;
        case 'C': return INPUT_KEY_RIGHT;
        case 'D': return INPUT_KEY_LEFT;
        default:  return -1;
    }
}

static int valid(input_key_t key) {
    return key >= 0 && key < INPUT_KEY_COUNT;
}

static void key_event(int k, int event, double now) {
    if(k < 0) return;

    if(!kitty) {
        last_seen[k] = now;
        return;
    }

    if(event == EVENT_RELEASE) {
        held[k] = 0;
    } else {
        if(!held[k]) tapped[k] = 1;
        held[k] = 1;
    }
}

// CSI = ESC [ parametros final. Kitty manda "ESC [ codigo ; mods : evento u"
// e as setas como "ESC [ 1 ; mods : evento A..D".
// Retorna os bytes consumidos, ou 0 se a sequencia ainda nao chegou inteira.
static size_t parse_csi(const unsigned char* s, size_t len, double now) {
    int field = 0, sub = 0;
    int code = 0, mods = 0, event = 0;

    for(size_t i = 2; i < len; i++) {
        unsigned char c = s[i];

        if(c >= '0' && c <= '9') {
            int digit = c - '0';
            if(field == 0 && sub == 0) code = code * 10 + digit;
            else if(field == 1 && sub == 0) mods = mods * 10 + digit;
            else if(field == 1 && sub == 1) event = event * 10 + digit;
        } else if(c == ';') {
            field++;
            sub = 0;
        } else if(c == ':') {
            sub++;
        } else if(c >= 0x40 && c <= 0x7E) {
            if(mods == 0) mods = 1;
            if(event == 0) event = EVENT_PRESS;

            if(c == 'u') {
                if(code == 'c' && ((mods - 1) & MOD_CTRL)) key_event(INPUT_KEY_ESC_QUIT, event, now);
                else key_event(map_char(code), event, now);
            } else if(c == 'O' && kitty) {
                // terminal perdeu o foco: os eventos de soltar nao vao chegar
                memset(held, 0, sizeof(held));
            } else {
                key_event(map_arrow(c), event, now);
            }
            return i + 1;
        }
    }

    // sequencia incompleta; se for lixo grande demais, descarta o ESC
    return len < 32 ? 0 : 1;
}

static size_t parse_one(const unsigned char* s, size_t len, double now) {
    if(s[0] != 27) {
        key_event(map_char(s[0]), EVENT_PRESS, now);
        return 1;
    }

    if(len == 1) {
        // no kitty o ESC sozinho vem como "ESC [ 27 u", entao um ESC solto e o comeco de uma sequencia
        if(kitty) return 0;
        key_event(INPUT_KEY_ESC_QUIT, EVENT_PRESS, now);
        return 1;
    }

    if(s[1] == '[') return parse_csi(s, len, now);

    if(s[1] == 'O') {
        if(len < 3) return 0;
        key_event(map_arrow(s[2]), EVENT_PRESS, now);
        return 3;
    }

    key_event(INPUT_KEY_ESC_QUIT, EVENT_PRESS, now);
    return 1;
}

// Pergunta as flags do kitty (CSI ? u) e os atributos do terminal (CSI c).
// Todo terminal responde o CSI c; so quem suporta o kitty responde o CSI ? u antes dele.
static int detect_kitty(void) {
    fputs("\033[?u\033[c", stdout);
    fflush(stdout);

    int supported = 0;
    double start = now_seconds();

    while(now_seconds() - start < DETECT_TIMEOUT_S) {
        if(!keyhit()) {
            struct timespec ts = {0, 1000000};
            nanosleep(&ts, NULL);
            continue;
        }

        int ch = readch();
        if(ch == 'u') supported = 1;
        if(ch == 'c') break;
    }

    return supported;
}

int input_init(void) {
    if(initialized) return 0;

    keyboardInit();
    kitty = detect_kitty();

    if(kitty) {
        // liga o protocolo e o aviso de foco (CSI I / CSI O)
        printf("\033[>%du\033[?1004h", KITTY_FLAGS);
        fflush(stdout);
    }

    for(int i = 0; i < INPUT_KEY_COUNT; i++) {
        down[i] = prev_down[i] = 0;
        held[i] = tapped[i] = 0;
        last_seen[i] = -1e9;
    }
    buf_len = 0;

    initialized = 1;
    return 0;
}

void input_update(void) {
    if(!initialized) return;

    double now = now_seconds();

    for(int i = 0; i < INPUT_KEY_COUNT; i++)
        prev_down[i] = down[i];

    while(buf_len < INPUT_BUF_SIZE && keyhit())
        buf[buf_len++] = (unsigned char)readch();

    size_t pos = 0;
    while(pos < buf_len) {
        size_t n = parse_one(buf + pos, buf_len - pos, now);
        if(n == 0) break;
        pos += n;
    }

    memmove(buf, buf + pos, buf_len - pos);
    buf_len -= pos;
    if(buf_len == INPUT_BUF_SIZE) buf_len = 0;

    for(int i = 0; i < INPUT_KEY_COUNT; i++) {
        if(kitty) down[i] = held[i] || tapped[i];
        else down[i] = (now - last_seen[i]) * 1000.0 < INPUT_HOLD_MS;
        tapped[i] = 0;
    }
}

void input_shutdown(void) {
    if(!initialized) return;

    if(kitty) {
        fputs("\033[<u\033[?1004l", stdout);
        fflush(stdout);
    }

    keyboardDestroy();
    initialized = 0;
}

int input_check_pressed(input_key_t key) {
    if(!valid(key)) return 0;
    return down[key] && !prev_down[key];
}

int input_check(input_key_t key) {
    if(!valid(key)) return 0;
    return down[key];
}

int input_check_released(input_key_t key) {
    if(!valid(key)) return 0;
    return !down[key] && prev_down[key];
}
