#include <math.h>
#include "game/player.h"
#include "engine/input.h"
#include "graphics/mesh.h"
#include "game/ship.h"
#include "math/mat4.h"

#define MAX_ROLL 0.5f
#define MAX_PITCH 0.3f
#define TILT_SPEED 8.0f

#define PLAYER_COLORS (ship_colors_t){ \
    .body = 0xFFFFFFFF,                \
    .detail = 0x3D7BFFFF,              \
    .engine = 0xFFC83DFF,              \
    .window = 0xFFFFFFFF               \
}

int player_init(player_t* player) {
    player->pos = (vec3_t){0, 0, 0};
    player->vel = (vec3_t){0, 0, 0};
    player->rot = (vec3_t){0, 0, 0};
    player->spd = 10.0f;

    mesh_t* mesh = ship_mesh_create(PLAYER_COLORS);

    if(!mesh) return 1;
    player->mesh = mesh;

    return 0;
}

static void player_move(player_t* player, float dt) {
    float horizontal = 0;
    float vertical = 0;

    if(key_down(SDL_SCANCODE_RIGHT)) horizontal++;
    if(key_down(SDL_SCANCODE_LEFT)) horizontal--;
    if(key_down(SDL_SCANCODE_UP)) vertical++;
    if(key_down(SDL_SCANCODE_DOWN)) vertical--;

    vec3_t dir = (vec3_t){ horizontal, 0.0f, vertical };
    player->pos = vec3_add(vec3_scale(dir, player->spd * dt), player->pos);

    float target_roll = horizontal * MAX_ROLL;
    float target_pitch = vertical * MAX_PITCH;

    float t = 1.0f - expf(-TILT_SPEED * dt);
    player->rot.x += (target_pitch - player->rot.x) * t;
    player->rot.y += (target_roll - player->rot.y) * t;
}

void player_update(player_t* player, float dt) {
    player_move(player, dt);
}

void player_draw(player_t* player, renderer_t* renderer, camera_t camera, light_t* lights, int light_count) {
    mat4_t model = mat4_identity();   
    model = mat4_mul(mat4_rotx(player->rot.x), model);
    model = mat4_mul(mat4_roty(player->rot.y), model);
    model = mat4_mul(mat4_translate(player->pos), model);

    renderer_draw_mesh(renderer, player->mesh, model, camera, lights, light_count);
}

void player_destroy(player_t* player) {
    mesh_destroy(player->mesh);
}
