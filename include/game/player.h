#ifndef PLAYER_H
#define PLAYER_H

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
} player_t;

int player_init(player_t* player);
void player_update(player_t* player, float dt);
void player_draw(player_t* player, renderer_t* renderer, camera_t camera, light_t* lights, int light_count);
void player_destroy(player_t* player);

#endif
