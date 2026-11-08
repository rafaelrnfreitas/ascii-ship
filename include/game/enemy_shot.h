#ifndef ENEMY_SHOT_H
#define ENEMY_SHOT_H

#include "math/vec3.h"
#include "engine/renderer.h"

typedef struct scene_s scene_t;

typedef struct {
    vec3_t pos;
    vec3_t vel;
    int active;
} enemy_shot_t;

void enemy_shot_fire(scene_t* scene, vec3_t from);
void enemy_shot_update(scene_t* scene, float dt);
void enemy_shot_draw(scene_t* scene, renderer_t* renderer);

#endif
