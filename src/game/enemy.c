#include "game/enemy.h"
#include "math/mat4.h"
#include "graphics/meshes.h"
#include "graphics/color.h"

#define PI 3.14159265f

int enemy_init(enemy_t* enemy, vec3_t pos) {
    mesh_t* src = mesh_ship;

    if(!src || !src->colors) return 1;

    mesh_t* mesh = mesh_create(
        src->verts, src->vert_count,
        src->indices, src->index_count,
        src->colors
    );
    if(!mesh) return 1;

    for(size_t i = 0; i < mesh->index_count / 3; i++) {
        mesh->colors[i] = COLOR_RED;
    }

    mesh->colors[2] = COLOR_CYAN;
    mesh->colors[38] = COLOR_CYAN;

    enemy->pos = pos;
    enemy->vel = (vec3_t){0, 0, 0};
    enemy->rot = (vec3_t){0, 0, PI};
    enemy->spd = 10.0f;
    enemy->mesh = mesh;

    return 0;
}

void enemy_draw(enemy_t* enemy, renderer_t* renderer, camera_t camera, light_t* lights, int light_count, vec3_t ambient) {
    mat4_t model = mat4_identity();
    model = mat4_mul(mat4_rotx(enemy->rot.x), model);
    model = mat4_mul(mat4_roty(enemy->rot.y), model);
    model = mat4_mul(mat4_rotz(enemy->rot.z), model);
    model = mat4_mul(mat4_translate(enemy->pos), model);

    renderer_draw_mesh(renderer, enemy->mesh, model, camera, lights, light_count, ambient);
}

void enemy_destroy(enemy_t* enemy) {
    mesh_destroy(enemy->mesh);
}
