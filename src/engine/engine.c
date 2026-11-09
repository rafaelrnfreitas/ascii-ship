#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/input.h"
#include "game/scene.h"
#include "game/lives.h"

#define TARGET_FRAME_NS (1000000000ull / 60)

int engine_init(engine_t* engine) {
    if(!engine) return 1;
    *engine = (engine_t){0};
   
    if(renderer_init(&engine->renderer, 384, 216, 1280, 720) != 0)
        return 1;
    if(lives_register() != 0 || scene_init(&engine->scene) != 0) {
        renderer_destroy(&engine->renderer);
        return 1;
    }
    engine->running = 1;
    return 0;
}

static void engine_handle(engine_t* engine, SDL_Event* e) {
    if(e->type == SDL_EVENT_QUIT) engine->running = 0;
    if(e->type != lives_event_type) return;
    if(e->user.code == LIVES_CHANGED) engine->hud_lives = (int)(intptr_t)e->user.data1;
    if(e->user.code == GAME_OVER) engine->game_over = 1;
}

static void engine_events(engine_t* engine) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) engine_handle(engine, &e);
    input_update();
}

void engine_update(engine_t* engine, float dt) {
    if(!engine || !engine->running) return;

    if(key_pressed(SDL_SCANCODE_ESCAPE)) {
        engine->running = 0;
        return;
    }
    if(engine->game_over) {
        if(key_pressed(SDL_SCANCODE_UP) || key_pressed(SDL_SCANCODE_DOWN) || key_pressed(SDL_SCANCODE_W) || key_pressed(SDL_SCANCODE_S))
            engine->menu_sel ^= 1;
        int pick = key_pressed(SDL_SCANCODE_RETURN) || key_pressed(SDL_SCANCODE_KP_ENTER);
        if(pick && engine->menu_sel == 1) { engine->running = 0; return; }
        if(pick || key_pressed(SDL_SCANCODE_R)) {
            scene_destroy(&engine->scene);
            if(scene_init(&engine->scene) != 0) { engine->running = 0; return; }
            engine->game_over = 0;
            engine->menu_sel = 0;
        }
    } else {
        scene_update(&engine->scene, dt);
    }

    // apply lives events pushed this frame before drawing, so the HUD never lags a frame
    SDL_Event e;
    while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, lives_event_type, lives_event_type) > 0)
        engine_handle(engine, &e);
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
