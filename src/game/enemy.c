#include "game/enemy.h"
#include "math/mat4.h"

#define PI 3.14159265f
#define ENEMY_RED 0xE53935FF
#define ENEMY_CYAN 0x00FFFFFF

static const int WINDOW_TRIS[] = {2, 38};

int enemy_init(enemy_t* enemy, vec3_t pos, const mesh_t* src) {
    if(!src || !src->colors) return 1;

    mesh_t* mesh = mesh_create(src->verts, src->vert_count, src->indices, src->index_count, src->colors);
    if(!mesh) return 1;

    for(size_t i = 0; i < mesh->index_count / 3; i++) {
        mesh->colors[i] = ENEMY_RED;
    }

    for(size_t i = 0; i < sizeof(WINDOW_TRIS) / sizeof(WINDOW_TRIS[0]); i++) {
        mesh->colors[WINDOW_TRIS[i]] = ENEMY_CYAN;
    }

    enemy->pos = pos;
    enemy->vel = (vec3_t){0, 0, 0};
    enemy->rot = (vec3_t){0, 0, PI};
    enemy->spd = 10.0f;
    enemy->mesh = mesh;

    return 0;
}

void enemy_draw(enemy_t* enemy, renderer_t* renderer, camera_t camera, light_t* lights, int light_count) {
    mat4_t model = mat4_identity();
    model = mat4_mul(mat4_rotx(enemy->rot.x), model);
    model = mat4_mul(mat4_roty(enemy->rot.y), model);
    model = mat4_mul(mat4_rotz(enemy->rot.z), model);
    model = mat4_mul(mat4_translate(enemy->pos), model);

    renderer_draw_mesh(renderer, enemy->mesh, model, camera, lights, light_count);
}

void enemy_destroy(enemy_t* enemy) {
    mesh_destroy(enemy->mesh);
}
