#ifndef ENGINE_H
#define ENGINE_H

#include "renderer.h"
#include "game/scene.h"
#include "game/leaderboard.h"

typedef enum { SCREEN_MENU, SCREEN_LEADERBOARD, SCREEN_PLAYING, SCREEN_ENTER_NAME, SCREEN_GAME_OVER } screen_t;

typedef struct {
    int running;
    screen_t screen;
    int hud_lives;
    int menu_sel;
    leaderboard_t leaderboard;
    char scores_path[512];
    renderer_t renderer;
    scene_t scene;
} engine_t;

int engine_init(engine_t* engine);
void engine_update(engine_t* engine, float dt);
void engine_draw(engine_t* engine);
void engine_run(engine_t* engine);
void engine_shutdown(engine_t* engine);

#endif
