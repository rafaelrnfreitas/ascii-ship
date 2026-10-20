#ifndef PLAYER_H
#define PLAYER_H

#include "math/vec3.h"
#include "graphics/mesh.h"
#include "engine/renderer.h"

typedef struct {
    vec3_t pos;
    vec3_t vel;
    float spd;
    mesh_t* mesh;
} player_t;

int player_init(player_t* player);
void player_update(player_t* player, float dt);
void player_draw(player_t* player, renderer_t* renderer, camera_t camera);
void player_destroy(player_t* player);

#endif
