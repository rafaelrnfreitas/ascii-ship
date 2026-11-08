#ifndef LASER_H
#define LASER_H

#include "math/vec3.h"
#include "graphics/mesh.h"
#include "engine/renderer.h"
#include "graphics/light.h"
#include "game/enemy.h"

typedef struct scene_s scene_t;

typedef struct {
    vec3_t pos;
    vec3_t vel;
    float spd;
    float dist;
    int active;
    float radius;
    scene_t* scene;
    mesh_t* mesh;
} laser_t;

int laser_init(laser_t* laser, vec3_t pos, scene_t* scene);
void laser_update(laser_t* laser, float dt);
void laser_draw(laser_t* laser, renderer_t* renderer, camera_t camera, light_t* lights, int light_count);

#endif
