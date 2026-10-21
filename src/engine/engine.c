#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/input.h"
#include "game/scene.h"

#define TARGET_FRAME_NS (1000000000ull / 60)

int engine_init(engine_t* engine) {
    if(!engine) return 1;
   
    if(renderer_init(&engine->renderer, 1280, 720) != 0)
        return 1;
    if(scene_init(&engine->scene) != 0) {
        renderer_destroy(&engine->renderer);
        return 1;
    }
    engine->running = 1;
    return 0;
}

static void engine_events(engine_t* engine) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_EVENT_QUIT)
            engine->running = 0;
    }
    input_update();
}

void engine_update(engine_t* engine, float dt) {
    if(!engine || !engine->running) return;

    if(key_pressed(SDL_SCANCODE_ESCAPE)) {
        engine->running = 0;
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
    Uint64 prev_time = SDL_GetTicksNS();

    while(engine->running) {
        Uint64 curr_time = SDL_GetTicksNS();
        float dt = (float)((double)(curr_time - prev_time) / 1e9);
        prev_time = curr_time;

        engine_events(engine);
        engine_update(engine, dt);
        engine_draw(engine);

        Uint64 elapsed = SDL_GetTicksNS() - curr_time;
        if(elapsed < TARGET_FRAME_NS) SDL_DelayNS(TARGET_FRAME_NS - elapsed);
    }
    engine_shutdown(engine);
}

void engine_shutdown(engine_t* engine) {
    if(!engine || !engine->running) return;

    scene_destroy(&engine->scene);
    renderer_destroy(&engine->renderer);
    engine->running = 0;
}
