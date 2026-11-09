#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "graphics/mesh.h"
#include "math/mat4.h"
#include "engine/input.h"
#include "game/scene.h"
#include "graphics/meshes.h"
#include "game/enemy_ai.h"

static float random_range(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

static void respawn_star(scene_t* scene, vec3_t* star) {
    star->x = scene->player.pos.x + random_range(-500, 500);
    star->y = scene->player.pos.y + random_range(200, 1000);
    star->z = scene->player.pos.z + random_range(-300, 300);
}

static int spawn_enemy(scene_t* scene, vec3_t pos) {
    if(scene->enemy_count >= MAX_ENEMIES) return 1;
    if(enemy_init(&scene->enemies[scene->enemy_count], pos, scene) != 0) return 1;
    scene->enemy_count++;
    return 0;
}

static void scene_spawn_flash(scene_t* scene, vec3_t pos, float size, float duration) {
    for(int i = 0; i < MAX_FLASHES; i++) {
        if(scene->flashes[i].active) continue;
        scene->flashes[i] = (flash_t){
            .pos = pos,
            .time = duration,
            .duration = duration,
            .size = size,
            .active = 1,
        };
        return;
    }
}

int scene_init(scene_t* scene) {
    if(!scene) return 1;
    
    meshes_init();

    memset(scene, 0, sizeof(scene_t));

    scene->ambient = (vec3_t){0.2f, 0.2f, 0.2f};
    scene->camera = (camera_t){
        .pos = {0, -10, 0}, 
        .target = {0, 0, 0},
        .near = 1.0f,
        .far = 100.0f,
        .fov = 1.0f,
    };

    screenshake_init(&scene->screenshake, scene);
    if(player_init(&scene->player, (vec3_t){0, 0, 0}, scene) != 0) return 1;
    
    spawn_enemy(scene, (vec3_t){0, 10, 0});
    spawn_enemy(scene, (vec3_t){10, 10, 0});
    spawn_enemy(scene, (vec3_t){-10, 10, 0});

    scene->lights[scene->light_count++] = light_create_dir(
        (vec3_t){0.5f, 0.5f, -1}, 0xFFFFFFFFU, 1.0f
    );

    for(int i = 0; i < NUM_STARS; i++) {
        respawn_star(scene, &scene->stars[i]);
    }

    return 0;
}

void scene_remove_enemy(scene_t* scene, int i) {
    enemy_destroy(&scene->enemies[i]);
    scene->enemies[i] = scene->enemies[--scene->enemy_count];
}

// An enemy ship that rams the player costs a life and is destroyed; during invulnerability it flies through.
static void scene_ram_player(scene_t* scene) {
    player_t* p = &scene->player;
    for(int i = 0; i < scene->enemy_count; i++) {
        float r = scene->enemies[i].radius + p->radius;
        if(scene->enemies[i].state == ENEMY_DYING) continue;
        if(vec3_dist_sq(scene->enemies[i].pos, p->pos) > r * r) continue;
        if(!lives_hit(&p->lives)) continue;

        scene_spawn_flash(scene, scene->enemies[i].pos, 3.0f, 0.2f);
        screenshake(&scene->screenshake, 0.6f, 0.3f, 1.5f);
        enemy_ai_kill(&scene->enemies[i]);
        return;
    }
}

void scene_update(scene_t* scene, float dt) {
    player_update(&scene->player, dt);

    screenshake_update(&scene->screenshake, dt);

    for(int i = 0; i < MAX_FLASHES; i++) {
        if(!scene->flashes[i].active) continue;
        scene->flashes[i].time -= dt;
        if(scene->flashes[i].time <= 0.0f) scene->flashes[i].active = 0;
    }
    for(int i = 0; i < scene->enemy_count; ) {
        if(enemy_ai_update(&scene->enemies[i], scene, dt)) scene_remove_enemy(scene, i);
        else i++;
    }
    for(int i = 0; i < MAX_LASERS; i++) {
        laser_update(&scene->lasers[i], dt);
    }
    scene_ram_player(scene);

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

    player_draw(&scene->player, renderer, scene->camera, scene->lights, scene->light_count, scene->ambient);

    for(int i = 0; i < scene->enemy_count; i++) {
        enemy_draw(&scene->enemies[i], renderer, scene->camera, scene->lights, scene->light_count, scene->ambient);
    }

    for(int i = 0; i < MAX_LASERS; i++) {
        laser_draw(&scene->lasers[i], renderer, scene->camera, NULL, 0);
    }

    for(int i = 0; i < MAX_FLASHES; i++) {
        flash_t* f = &scene->flashes[i];
        if(!f->active) continue;

        float size = f->size * (f->time / f->duration);   // 1 -> 0

        mat4_t m = mat4_identity();
        m = mat4_mul(mat4_scale((vec3_t){size, size, size}), m);
        m = mat4_mul(mat4_translate(f->pos), m);

        renderer_draw_mesh(renderer, mesh_circle, m, scene->camera, NULL, 0, (vec3_t){1, 1, 1});
    }
}

void scene_destroy(scene_t* scene) {
    if(!scene) return;
    
    meshes_free();

    for(int i = 0; i < scene->enemy_count; i++) {
        enemy_destroy(&scene->enemies[i]);
    }
    player_destroy(&scene->player);
}
