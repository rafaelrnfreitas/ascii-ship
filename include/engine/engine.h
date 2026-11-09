#ifndef ENGINE_H
#define ENGINE_H

#include "renderer.h"
#include "game/scene.h"

typedef struct {
    int running;
    int game_over;
    int hud_lives;
    int menu_sel;   // game over menu: 0 restart, 1 quit
    renderer_t renderer;
    scene_t scene;
} engine_t;

int engine_init(engine_t* engine);
void engine_update(engine_t* engine, float dt);
void engine_draw(engine_t* engine);
void engine_run(engine_t* engine);
void engine_shutdown(engine_t* engine);

#endif
