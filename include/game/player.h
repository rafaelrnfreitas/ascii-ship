#ifndef PLAYER_H
#define PLAYER_H

#include "math/vec3.h"
#include "graphics/mesh.h"
#include "engine/renderer.h"
#include "graphics/light.h"
#include "game/laser.h"
#include "game/lives.h"

typedef struct scene_s scene_t;

typedef struct {
    vec3_t pos;
    vec3_t vel;
    vec3_t rot;
    float spd;
    float fire_cooldown;
    mesh_t* mesh;
    float radius;
    scene_t* scene;
    lives_t lives;
} player_t;

int player_init(player_t* player, vec3_t pos, scene_t* scene);
void player_update(player_t* player, float dt); 
void player_draw(player_t* player, renderer_t* renderer, camera_t camera, light_t* lights, int light_count, vec3_t ambient);
void player_destroy(player_t* player);

#endif
