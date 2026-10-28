#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "graphics/mesh.h"
#include "math/mat4.h"
#include "engine/input.h"
#include "game/scene.h"

static float random_range(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

static void respawn_star(scene_t* scene, vec3_t* star) {
    star->x = scene->player.pos.x + random_range(-500, 500);
    star->y = scene->player.pos.y + random_range(200, 1000);
    star->z = scene->player.pos.z + random_range(-300, 300);
}

int scene_init(scene_t* scene) {
    if(!scene) return 1;

    memset(scene, 0, sizeof(scene_t));

    scene->camera = (camera_t){
        .pos = {0, -10, 0}, 
        .target = {0, 0, 0},
        .near = 1.0f,
        .far = 100.0f,
        .fov = 1.0f
    };
    player_init(&scene->player);
    scene->lights[scene->light_count++] = light_create_dir(
                (vec3_t){0.5f, 0.5f, -1}, 0xFFFFFFFFU, 1.0f
            );

    for(int i = 0; i < NUM_STARS; i++) {
        respawn_star(scene, &scene->stars[i]);
    }

    return 0;
}

void scene_update(scene_t* scene, float dt) {
    player_update(&scene->player, dt);

    for(int i = 0; i < NUM_STARS; i++) {
        scene->stars[i] = vec3_add(scene->stars[i], (vec3_t){0, -400.0 * dt, 0});
        if(scene->stars[i].y < scene->player.pos.y) {
            respawn_star(scene, &scene->stars[i]);
        }
    }
}

void scene_draw(scene_t* scene, renderer_t* renderer) {
    float focal_length = (renderer->width * 0.5f) / tanf(scene->camera.fov * 0.5f);

    for(int i = 0; i < NUM_STARS; i++) {
        vec3_t star = scene->stars[i];

        float depth = star.y - scene->camera.pos.y;

        if(depth <= scene->camera.near) continue;

        float x = renderer->width * 0.5f
                + (star.x - scene->camera.pos.x)
                * focal_length / depth;

        float y = renderer->height * 0.5f
                - (star.z - scene->camera.pos.z)
                * focal_length / depth;

        if(x < 0 || x >= renderer->width || y < 0 || y >= renderer->height) continue;

        renderer_draw_pixel(renderer, (int)x, (int)y, 0xFFFFFFFFU);
    }

    player_draw(&scene->player, renderer, scene->camera, scene->lights, scene->light_count);
}

void scene_destroy(scene_t* scene) {
    if(!scene) return;
    player_destroy(&scene->player);
}
