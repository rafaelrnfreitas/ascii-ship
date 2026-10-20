#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/input.h"
#include "game/scene.h"

static double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int engine_init(engine_t* engine) {
    if(!engine) return 1;
    if(input_init() != 0) return 1;
    if(renderer_init(&engine->renderer, 80, 24) != 0) {
        input_shutdown();
        return 1;
    }
    if(scene_init(&engine->scene) != 0) {
        renderer_destroy(&engine->renderer);
        input_shutdown();
        return 1;
    }
    engine->running = 1;
    return 0;
}

void engine_update(engine_t* engine, float dt) {
    if(!engine || !engine->running) return;

    input_update();
    if(input_check_pressed(INPUT_KEY_ESC_QUIT)) {
        engine_shutdown(engine);
        return;
    }
    scene_update(&engine->scene, dt);
}

void engine_draw(engine_t* engine) {
    if(!engine || !engine->running) return;
    
    renderer_clear(&engine->renderer);
    scene_draw(&engine->scene, &engine->renderer);
    renderer_draw(&engine->renderer);
}

void engine_run(engine_t* engine) {
    double prev_time = get_time();

    while(engine->running) {
        double curr_time = get_time();
        float dt = (float)(curr_time - prev_time);
        prev_time = curr_time;

        engine_update(engine, dt);
        engine_draw(engine);
    
        struct timespec ts = {0, 33000000};
        nanosleep(&ts, NULL);
    }
}

void engine_shutdown(engine_t* engine) {
    if(!engine || !engine->running) return;

    scene_destroy(&engine->scene);
    renderer_destroy(&engine->renderer);
    input_shutdown();
    engine->running = 0;
}
