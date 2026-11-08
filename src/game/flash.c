#include "game/flash.h"
#include "math/mat4.h"
#include "graphics/meshes.h"

void flash_init(flash_t* flash, vec3_t pos, float size, float duration) {
    flash->pos = pos;
    flash->time = duration;
    flash->duration = duration;
    flash->size = size;
    flash->active = 1;
    flash->mesh = mesh_circle;
}

void flash_update(flash_t* flash, float dt) {
    if(!flash->active) return;

    flash->time -= dt;
    if(flash->time <= 0.0f) flash->active = 0;
}

void flash_draw(flash_t* flash, renderer_t* renderer, camera_t camera) {
    if(!flash->active) return;

    float size = flash->size * (flash->time / flash->duration);

    mat4_t model = mat4_identity();
    model = mat4_mul(mat4_scale((vec3_t){size, size, size}), model);
    model = mat4_mul(mat4_translate(flash->pos), model);

    renderer_draw_mesh(renderer, flash->mesh, model, camera, NULL, 0, (vec3_t){1, 1, 1});
}
