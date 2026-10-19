#ifndef SCENE_H
#define SCENE_H

#include "graphics/mesh.h"
#include "graphics/camera.h"
#include "game/player.h"
#include "engine/renderer.h"

#define MAX_MESHES 64

typedef struct {
    mesh_t* meshes[MAX_MESHES];
    size_t mesh_count;
    camera_t camera;
    player_t player;
} scene_t;

int scene_init(scene_t* scene);
void scene_update(scene_t* scene, float dt);
void scene_draw(scene_t* scene, renderer_t* renderer);
void scene_destroy(scene_t* scene);

#endif
