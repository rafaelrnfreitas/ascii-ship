#include "game/enemy.h"
#include "game/ship.h"
#include "math/mat4.h"

#define ENEMY_COLORS (ship_colors_t){ \
    .body = 0xE53935FF,               \
    .detail = 0xFFFFFFFF,             \
    .engine = 0xFFC83DFF,             \
    .window = 0x8FD3FFFF              \
}

int enemy_init(enemy_t* enemy, vec3_t pos) {
    enemy->pos = pos;
    enemy->vel = (vec3_t){0, 0, 0};
    enemy->rot = (vec3_t){0, 0, 3.14159265f};
    enemy->spd = 10.0f;

    mesh_t* mesh = ship_mesh_create(ENEMY_COLORS);

    if(!mesh) return 1;
    enemy->mesh = mesh;

    return 0;
}

void enemy_update(enemy_t* enemy, float dt) {
    (void)enemy;
    (void)dt;
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
