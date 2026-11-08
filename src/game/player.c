#include <math.h>
#include "game/player.h"
#include "engine/input.h"
#include "graphics/mesh.h"
#include "math/mat4.h"
#include "graphics/meshes.h"
#include "game/scene.h"

#define MAX_ROLL 0.5f
#define MAX_PITCH 0.3f
#define TILT_SPEED 8.0f
#define FIRE_RATE 0.15f
#define CANNON_OFFSET ((vec3_t){0, 2.5f, -0.2f})
#define FLASH_TIME 0.08f
#define FLASH_SIZE 0.9f

int player_init(player_t* player, vec3_t pos, scene_t* scene) {
    player->pos = pos;
    player->vel = (vec3_t){0, 0, 0};
    player->rot = (vec3_t){0, 0, 0};
    player->spd = 10.0f;
    player->fire_cooldown = 0;

    player->mesh = mesh_ship;
    player->radius = 2.0f;
    player->scene = scene;
    lives_reset(&player->lives);

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
    lives_update(&player->lives, dt);

    if(player->fire_cooldown > 0) {
        player->fire_cooldown -= dt;
    } else {
        if(key_down(SDL_SCANCODE_SPACE)) {
            player->fire_cooldown = FIRE_RATE;
            screenshake(&player->scene->screenshake, 0.15f, 0.1f, 1.5f);

            vec3_t cannon = vec3_add(player->pos, CANNON_OFFSET);

            for(int i = 0; i < MAX_LASERS; i++) {
                if(player->scene->lasers[i].active) continue;
                laser_init(&player->scene->lasers[i], cannon, player->scene);
                break;
            }

            for(int i = 0; i < MAX_FLASHES; i++) {
                if(player->scene->flashes[i].active) continue;
                flash_init(&player->scene->flashes[i], cannon, FLASH_SIZE, FLASH_TIME);
                break;
            }
        }
    }
}

void player_draw(player_t* player, renderer_t* renderer, camera_t camera, light_t* lights, int light_count, vec3_t ambient) {
    if(!lives_visible(&player->lives)) return;

    mat4_t model = mat4_identity();   
    model = mat4_mul(mat4_rotx(player->rot.x), model);
    model = mat4_mul(mat4_roty(player->rot.y), model);
    model = mat4_mul(mat4_translate(player->pos), model);

    renderer_draw_mesh(renderer, player->mesh, model, camera, lights, light_count, ambient);
}

void player_destroy(player_t* player) {
}
