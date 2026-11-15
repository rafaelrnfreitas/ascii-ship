#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/input.h"
#include "game/scene.h"
#include "game/lives.h"
#include "game/tuning.h"
#include "graphics/font.h"
#include "graphics/color.h"

#define TARGET_FRAME_NS (1000000000ull / 60)

int engine_init(engine_t* engine) {
    if(!engine) return 1;
    *engine = (engine_t){0};
    srand(GAME_SEED ? GAME_SEED : (unsigned)time(NULL));

    if(renderer_init(&engine->renderer, 384, 216, 1280, 720) != 0)
        return 1;
    if(lives_register() != 0 || scene_init(&engine->scene) != 0) {
        renderer_destroy(&engine->renderer);
        return 1;
    }
    const char* base = SDL_GetBasePath();
    snprintf(engine->scores_path, sizeof engine->scores_path, "%sscores.txt", base ? base : "");
    leaderboard_load(&engine->leaderboard, engine->scores_path);
    engine->screen = SCREEN_MENU;
    engine->running = 1;
    return 0;
}

static void save_score(engine_t* engine) {
    leaderboard_insert(&engine->leaderboard, engine->name, engine->scene.score);
    leaderboard_save(&engine->leaderboard, engine->scores_path);
}

static void quit(engine_t* engine) {
    if(engine->screen == SCREEN_ENTER_NAME) save_score(engine);
    engine->running = 0;
}

static void game_over(engine_t* engine) {
    if(!leaderboard_qualifies(&engine->leaderboard, engine->scene.score)) {
        engine->screen = SCREEN_GAME_OVER;
        return;
    }
    strcpy(engine->name, "AAA");
    engine->name_pos = 0;
    engine->screen = SCREEN_ENTER_NAME;
}

static void engine_handle(engine_t* engine, SDL_Event* e) {
    if(e->type == SDL_EVENT_QUIT) quit(engine);
    if(e->type != lives_event_type) return;
    if(e->user.code == LIVES_CHANGED) engine->hud_lives = (int)(intptr_t)e->user.data1;
    if(e->user.code == GAME_OVER) game_over(engine);
}

static void engine_events(engine_t* engine) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) engine_handle(engine, &e);
    input_update();
}

static int enter_pressed(void) {
    return key_pressed(SDL_SCANCODE_RETURN) || key_pressed(SDL_SCANCODE_KP_ENTER);
}

static int menu_input(engine_t* engine) {
    if(key_pressed(SDL_SCANCODE_UP) || key_pressed(SDL_SCANCODE_DOWN) || key_pressed(SDL_SCANCODE_W) || key_pressed(SDL_SCANCODE_S))
        engine->menu_sel ^= 1;
    return enter_pressed();
}

static void new_game(engine_t* engine) {
    scene_destroy(&engine->scene);
    if(scene_init(&engine->scene) != 0) { engine->running = 0; return; }
    engine->screen = SCREEN_PLAYING;
    engine->menu_sel = 0;
}

void engine_update(engine_t* engine, float dt) {
    if(!engine || !engine->running) return;

    if(key_pressed(SDL_SCANCODE_ESCAPE) && engine->screen != SCREEN_LEADERBOARD) {
        quit(engine);
        return;
    }

    switch(engine->screen) {
    case SCREEN_MENU:
        if(menu_input(engine)) {
            if(engine->menu_sel == 0) new_game(engine);
            else engine->screen = SCREEN_LEADERBOARD;
        }
        break;
    case SCREEN_LEADERBOARD:
        if(key_pressed(SDL_SCANCODE_ESCAPE) || enter_pressed()) engine->screen = SCREEN_MENU;
        break;
    case SCREEN_PLAYING:
        scene_update(&engine->scene, dt);
        break;
    case SCREEN_ENTER_NAME: {
        char* c = &engine->name[engine->name_pos];
        if(key_pressed(SDL_SCANCODE_UP) || key_pressed(SDL_SCANCODE_W)) *c = *c == 'Z' ? 'A' : *c + 1;
        if(key_pressed(SDL_SCANCODE_DOWN) || key_pressed(SDL_SCANCODE_S)) *c = *c == 'A' ? 'Z' : *c - 1;
        if(key_pressed(SDL_SCANCODE_RIGHT) || key_pressed(SDL_SCANCODE_D)) engine->name_pos = (engine->name_pos + 1) % 3;
        if(key_pressed(SDL_SCANCODE_LEFT) || key_pressed(SDL_SCANCODE_A)) engine->name_pos = (engine->name_pos + 2) % 3;
        if(enter_pressed()) {
            save_score(engine);
            engine->screen = SCREEN_GAME_OVER;
        }
        break;
    }
    case SCREEN_GAME_OVER: {
        int pick = menu_input(engine);
        if(pick && engine->menu_sel == 1) { engine->running = 0; return; }
        if(pick || key_pressed(SDL_SCANCODE_R)) new_game(engine);
        break;
    }
    }

    SDL_Event e;
    while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, lives_event_type, lives_event_type) > 0)
        engine_handle(engine, &e);
}

static void draw_centered(engine_t* engine, int y, const char* text, int scale, uint32_t color) {
    font_draw(&engine->renderer, (engine->renderer.width - font_width(text, scale)) / 2, y, text, scale, color);
}

static void draw_menu(engine_t* engine, const char* title, uint32_t title_color, const char* items[2]) {
    int h = engine->renderer.height;
    draw_centered(engine, h / 2 - 30, title, 3, title_color);
    for(int i = 0; i < 2; i++)
        draw_centered(engine, h / 2 + 14 + i * 20, items[i], 2, engine->menu_sel == i ? COLOR_YELLOW : COLOR_GRAY);
}

static void draw_leaderboard(engine_t* engine) {
    leaderboard_t* lb = &engine->leaderboard;
    char row[32];

    draw_centered(engine, 20, "LEADERBOARD", 2, COLOR_YELLOW);
    if(lb->count == 0) draw_centered(engine, 60, "NO SCORES YET", 1, COLOR_GRAY);
    for(int i = 0; i < lb->count; i++) {
        snprintf(row, sizeof row, "%2d  %s  %7d", i + 1, lb->entries[i].name, lb->entries[i].score);
        draw_centered(engine, 50 + i * 12, row, 1, COLOR_WHITE);
    }
    draw_centered(engine, engine->renderer.height - 20, "PRESS ENTER", 1, COLOR_GRAY);
}

static void draw_enter_name(engine_t* engine) {
    int h = engine->renderer.height;
    draw_centered(engine, h / 2 - 50, "NEW HIGH SCORE", 2, COLOR_YELLOW);
    draw_centered(engine, h / 2 - 25, "TYPE YOUR INITIALS", 1, COLOR_GRAY);

    int x = (engine->renderer.width - font_width("AAA", 3)) / 2;
    for(int i = 0; i < 3; i++) {
        char letter[2] = { engine->name[i], 0 };
        font_draw(&engine->renderer, x + i * 18, h / 2 - 5, letter, 3, engine->name_pos == i ? COLOR_YELLOW : COLOR_WHITE);
    }
    draw_centered(engine, h / 2 + 30, "PRESS ENTER TO SAVE", 1, COLOR_GRAY);
}

static void draw_hud(engine_t* engine) {
    char hud[16];
    snprintf(hud, sizeof hud, "LIVES %d", engine->hud_lives);
    font_draw(&engine->renderer, 4, 4, hud, 1, COLOR_WHITE);
    snprintf(hud, sizeof hud, "SCORE %d", engine->scene.score);
    font_draw(&engine->renderer, engine->renderer.width - 4 - font_width(hud, 1), 4, hud, 1, COLOR_WHITE);
}

void engine_draw(engine_t* engine) {
    if(!engine || !engine->running) return;

    renderer_clear(&engine->renderer);
    scene_draw(&engine->scene, &engine->renderer);

    switch(engine->screen) {
    case SCREEN_MENU:
        draw_menu(engine, "ASCII SHIP", COLOR_WHITE, (const char*[]){ "PLAY", "LEADERBOARD" });
        break;
    case SCREEN_LEADERBOARD:
        draw_leaderboard(engine);
        break;
    case SCREEN_PLAYING:
        draw_hud(engine);
        break;
    case SCREEN_ENTER_NAME:
        draw_hud(engine);
        draw_enter_name(engine);
        break;
    case SCREEN_GAME_OVER:
        draw_hud(engine);
        draw_menu(engine, "GAME OVER", COLOR_RED, (const char*[]){ "RESTART", "QUIT" });
        break;
    }
    renderer_draw(&engine->renderer);
}

void engine_run(engine_t* engine) {
    Uint64 prev_time = SDL_GetTicksNS();

    while(engine->running) {
        Uint64 curr_time = SDL_GetTicksNS();
        float dt = (float)((double)(curr_time - prev_time) / 1e9);
        prev_time = curr_time;
        if(dt > 0.05f) dt = 0.05f;

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
