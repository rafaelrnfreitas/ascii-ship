#include <string.h>
#include "graphics/mesh.h"
#include "math/mat4.h"
#include "engine/input.h"
#include "game/scene.h"

int scene_init(scene_t* scene) {
    if(!scene) return 1;

    memset(scene, 0, sizeof(scene_t));

    scene->camera = (camera_t){{0, -20, 0}, {0, 0, 0}};
    player_init(&scene->player);
    scene->lights[scene->light_count++] = light_create_dir(
                (vec3_t){0.5f, 0.5f, -1}, 0xFFFFFFFFU, 1.0f
            );

    return 0;
}

void scene_update(scene_t* scene, float dt) {
    player_update(&scene->player, dt);
}

void scene_draw(scene_t* scene, renderer_t* renderer) {
    player_draw(&scene->player, renderer, scene->camera, scene->lights, scene->light_count);
}

void scene_destroy(scene_t* scene) {
    if(!scene) return;
    player_destroy(&scene->player);
}
