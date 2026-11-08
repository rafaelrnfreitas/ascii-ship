#include "game/laser.h"
#include "math/mat4.h"
#include "graphics/meshes.h"
#include "game/scene.h" 

#define LASER_MAX_DIST 80.0f
#define FLASH_SIZE 2.0f
#define FLASH_TIME 0.15f

int laser_init(laser_t* laser, vec3_t pos, scene_t* scene) {
    laser->pos = pos;
    laser->vel = (vec3_t){0, 1, 0};
    laser->spd = 60.0f;
    laser->dist = 0;
    laser->active = 1;
    laser->mesh = mesh_laser;
    laser->radius = 1.0f;
    laser->scene = scene;

    return 0;
}

void laser_update(laser_t* laser, float dt) {
    if(!laser->active) return;

    laser->pos = vec3_add(vec3_scale(laser->vel, laser->spd * dt), laser->pos);
    laser->dist += laser->spd * dt;

    for(int i = 0; i < laser->scene->enemy_count; i++) {
        float r = laser->scene->enemies[i].radius + laser->radius;

        if(vec3_dist_sq(laser->scene->enemies[i].pos, laser->pos) <= r * r) {
            enemy_destroy(&laser->scene->enemies[i]);
            laser->scene->enemies[i] = laser->scene->enemies[laser->scene->enemy_count - 1];
            laser->scene->enemy_count--;
            laser->active = 0;

            for(int i = 0; i < MAX_FLASHES; i++) {
                if(laser->scene->flashes[i].active) continue;
                flash_init(&laser->scene->flashes[i], laser->pos, FLASH_SIZE, FLASH_TIME);
                break;
            }

            return;
        }
    }

    if(laser->dist > LASER_MAX_DIST) laser->active = 0;
}

void laser_draw(laser_t* laser, renderer_t* renderer, camera_t camera, light_t* lights, int light_count) {
    if(!laser->active) return;

    mat4_t model = mat4_translate(laser->pos);

    renderer_draw_mesh(renderer, laser->mesh, model, camera, lights, light_count, (vec3_t){1.0f, 1.0f, 1.0f});
}
