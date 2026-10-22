#ifndef ENEMY_H
#define ENEMY_H

#include "math/vec3.h"
#include "graphics/mesh.h"
#include "engine/renderer.h"
#include "graphics/light.h"

typedef struct {
    vec3_t pos;
    vec3_t vel;
    vec3_t rot;
    float spd;
    mesh_t* mesh;
} enemy_t;

int enemy_init(enemy_t* enemy, vec3_t pos);
void enemy_update(enemy_t* enemy, float dt);
void enemy_draw(enemy_t* enemy, renderer_t* renderer, camera_t camera, light_t* lights, int light_count);
void enemy_destroy(enemy_t* enemy);

#endif
