#include <stdio.h>
#include <time.h>
#include <string.h>
#include "engine.h"

static double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int engine_init(engine_t* engine) {
    if(!engine) return 1;
    engine->running = 1;

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            engine->screen_buffer[y][x] = ' ';
        }
    }

    return 0;
}

void engine_update(engine_t* engine, float dt) {
    engine_draw_char(engine, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 'X');
}

void engine_draw(engine_t* engine) {
    if (!engine) return;

    printf("\033[H");

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            putchar(engine->screen_buffer[y][x]);
        }
        putchar('\n');
    }

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            engine->screen_buffer[y][x] = ' ';
        }
    }
}

void engine_draw_char(engine_t* engine, int x, int y, char c) {
    if (!engine) return;

    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        engine->screen_buffer[y][x] = c;
    }
}

void engine_run(engine_t* engine) {
    double prev_time = get_time();

    printf("\033[2J\033[?25l");

    while(engine->running) {
        double curr_time = get_time();
        float dt = (float)(curr_time - prev_time);
        prev_time = curr_time;

        engine_update(engine, dt);
        engine_draw(engine);
    }
}

void engine_shutdown(engine_t* engine) {
    if(!engine) return;
    engine->running = 0;

    printf("\033[?25h");
}
