#include <stdio.h>
#include <time.h>
#include <string.h>
#include "engine/engine.h"

static double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int engine_init(engine_t* engine) {
    if(!engine) return 1;
    engine->running = 1;

    renderer_clear(engine->renderer);

    return 0;
}

void engine_update(engine_t* engine, float dt) {
}

void engine_draw(engine_t* engine) {
    if (!engine) return;

    renderer_draw_char(engine->renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 'X');
    renderer_clear(engine->renderer);
}

void engine_run(engine_t* engine) {
    double prev_time = get_time();

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
