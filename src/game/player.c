#include <math.h>
#include "game/player.h"
#include "engine/input.h"
#include "graphics/mesh.h"
#include "math/mat4.h"

int player_init(player_t* player) {
    player->pos = (vec3_t){0, 0, 0};
    player->vel = (vec3_t){0, 0, 0};
    player->spd = 6.0f;

    vertex_t verts[] = {
        {{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},

        {{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},

        {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},

        {{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}},

        {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},

        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}}
    };

    int indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    mesh_t* cube = mesh_create(
        verts,
        sizeof(verts) / sizeof(verts[0]),
        indices,
        sizeof(indices) / sizeof(indices[0])
    );

    if(!cube) return 1;
    player->mesh = cube;

    return 0;
}

static void player_move(player_t* player, float dt) {
    float horizontal = 0; 
    float vertical = 0;

    if(key_down(SDL_SCANCODE_D)) horizontal++;
    if(key_down(SDL_SCANCODE_A)) horizontal--;
    if(key_down(SDL_SCANCODE_W)) vertical++;
    if(key_down(SDL_SCANCODE_S)) vertical--;

    vec3_t dir =(vec3_t){
        horizontal,
        0.0f,
        vertical
    };
    
    player->pos = vec3_add(vec3_scale(dir, player->spd * dt), player->pos); 
}

void player_update(player_t* player, float dt) {
    player_move(player, dt);
}

void player_draw(player_t* player, renderer_t* renderer, camera_t camera, light_t* lights, int light_count) {
    mat4_t model = mat4_identity();   
    model = mat4_mul(mat4_translate(player->pos), model);

    renderer_draw_mesh(renderer, player->mesh, model, camera, lights, light_count);
}

void player_destroy(player_t* player) {
    mesh_destroy(player->mesh);
}
