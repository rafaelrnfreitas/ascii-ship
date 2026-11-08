#ifndef SCENE_H
#define SCENE_H

#include "graphics/mesh.h"
#include "graphics/camera.h"
#include "game/player.h"
#include "game/enemy.h"
#include "game/laser.h"
#include "engine/renderer.h"
#include "graphics/light.h"
#include "game/screenshake.h"

#define MAX_MESHES 64
#define NUM_STARS 256
#define MAX_LASERS 32
#define MAX_ENEMIES 16

typedef struct scene_s {
    camera_t camera;
    player_t player;
    enemy_t enemies[MAX_ENEMIES];
    int enemy_count;
    light_t lights[MAX_LIGHTS];
    vec3_t stars[NUM_STARS];
    laser_t lasers[MAX_LASERS];
    int light_count;
    vec3_t ambient;
    screenshake_t screenshake;
} scene_t;

int scene_init(scene_t* scene);
void scene_update(scene_t* scene, float dt);
void scene_draw(scene_t* scene, renderer_t* renderer);
void scene_destroy(scene_t* scene);

#endif
