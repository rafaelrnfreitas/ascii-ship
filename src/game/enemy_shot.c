#include <math.h>
#include "game/enemy_shot.h"
#include "game/scene.h"
#include "game/tuning.h"
#include "graphics/meshes.h"
#include "math/mat4.h"

// Aims at where the player is right now, no prediction. No free slot: the shot is dropped (hard cap).
void enemy_shot_fire(scene_t* scene, vec3_t from) {
    for(int i = 0; i < ENEMY_SHOT_CAP; i++) {
        enemy_shot_t* s = &scene->enemy_shots[i];
        if(s->active) continue;
        s->pos = from;
        s->vel = vec3_scale(vec3_normalize(vec3_sub(scene->player.pos, from)), ENEMY_SHOT_SPEED);
        s->active = 1;
        return;
    }
}

void enemy_shot_update(scene_t* scene, float dt) {
    player_t* p = &scene->player;
    float r = ENEMY_SHOT_RADIUS + p->radius;

    for(int i = 0; i < ENEMY_SHOT_CAP; i++) {
        enemy_shot_t* s = &scene->enemy_shots[i];
        if(!s->active) continue;

        s->pos = vec3_add(s->pos, vec3_scale(s->vel, dt));

        if(s->pos.y < -12.0f || s->pos.y > 120.0f || fabsf(s->pos.x) > 60.0f || fabsf(s->pos.z) > 40.0f) {
            s->active = 0;
            continue;
        }
        if(vec3_dist_sq(s->pos, p->pos) > r * r) continue;

        s->active = 0;   // shots are destroyed even when the player is invulnerable
        if(lives_hit(&p->lives)) screenshake(&scene->screenshake, 0.6f, 0.3f, 1.5f);
    }
}

void enemy_shot_draw(scene_t* scene, renderer_t* renderer) {
    for(int i = 0; i < ENEMY_SHOT_CAP; i++) {
        enemy_shot_t* s = &scene->enemy_shots[i];
        if(!s->active) continue;

        mat4_t m = mat4_identity();
        m = mat4_mul(mat4_scale((vec3_t){0.6f, 0.6f, 0.6f}), m);
        m = mat4_mul(mat4_translate(s->pos), m);

        renderer_draw_mesh(renderer, mesh_circle, m, scene->camera, NULL, 0, (vec3_t){1, 1, 1});
    }
}
