#include <ncurses.h>
#include <time.h>
#include "engine/input.h"

static int initialized = 0;
static int down[INPUT_KEY_COUNT];
static int prev_down[INPUT_KEY_COUNT];
static double last_seen[INPUT_KEY_COUNT];

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static int map_key(int ch) {
    switch(ch) {
        case KEY_RIGHT: return INPUT_KEY_RIGHT;
        case KEY_LEFT:  return INPUT_KEY_LEFT;
        case KEY_UP:    return INPUT_KEY_UP;
        case KEY_DOWN:  return INPUT_KEY_DOWN;
        case 'w': case 'W': return INPUT_KEY_W;
        case 'a': case 'A': return INPUT_KEY_A;
        case 's': case 'S': return INPUT_KEY_S;
        case 'd': case 'D': return INPUT_KEY_D;
        case ' ':  return INPUT_KEY_SPACE;
        case '\n': case '\r': case KEY_ENTER: return INPUT_KEY_ENTER;
        case 27:   return INPUT_KEY_ESC_QUIT;
        default:   return -1;
    }
}

static int valid(input_key_t key) {
    return key >= 0 && key < INPUT_KEY_COUNT;
}

int input_init(void) {
    if(initialized) return 0;
    if(initscr() == NULL) return 1;

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    set_escdelay(25);
    curs_set(0);

    for(int i = 0; i < INPUT_KEY_COUNT; i++) {
        down[i] = prev_down[i] = 0;
        last_seen[i] = -1e9;
    }

    initialized = 1;
    return 0;
}

void input_update(void) {
    if(!initialized) return;

    double now = now_seconds();

    for(int i = 0; i < INPUT_KEY_COUNT; i++)
        prev_down[i] = down[i];

    int ch;
    while((ch = getch()) != ERR) {
        int k = map_key(ch);
        if(k >= 0) last_seen[k] = now;
    }

    for(int i = 0; i < INPUT_KEY_COUNT; i++)
        down[i] = (now - last_seen[i]) * 1000.0 < INPUT_HOLD_MS;
}

void input_shutdown(void) {
    if(!initialized) return;
    endwin();
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