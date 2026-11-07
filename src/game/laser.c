#include "game/laser.h"
#include "math/mat4.h"
#include "graphics/meshes.h"

#define LASER_MAX_DIST 80.0f

int laser_init(laser_t* laser, vec3_t pos) {
    laser->pos = pos;
    laser->vel = (vec3_t){0, 1, 0};
    laser->spd = 60.0f;
    laser->dist = 0;
    laser->active = 1;
    laser->mesh = mesh_laser;

    return 0;
}

void laser_update(laser_t* laser, float dt) {
    if(!laser->active) return;

    laser->pos = vec3_add(vec3_scale(laser->vel, laser->spd * dt), laser->pos);
    laser->dist += laser->spd * dt;

    if(laser->dist > LASER_MAX_DIST) laser->active = 0;
}

void laser_draw(laser_t* laser, renderer_t* renderer, camera_t camera, light_t* lights, int light_count) {
    if(!laser->active) return;

    mat4_t model = mat4_translate(laser->pos);

    renderer_draw_mesh(renderer, laser->mesh, model, camera, lights, light_count, (vec3_t){1.0f, 1.0f, 1.0f});
}
