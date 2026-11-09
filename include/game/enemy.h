#ifndef ENEMY_H
#define ENEMY_H

#include "math/vec3.h"
#include "graphics/mesh.h"
#include "engine/renderer.h"
#include "graphics/light.h"

typedef struct scene_s scene_t;

typedef enum { ENEMY_DRIFTER, ENEMY_CHASER, ENEMY_SHOOTER } enemy_type_t;
typedef enum { ENEMY_ENTERING, ENEMY_ACTIVE, ENEMY_LEAVING, ENEMY_DYING } enemy_state_t;

typedef struct {
    vec3_t pos;
    vec3_t vel;
    vec3_t rot;
    float spd;
    float radius;
    mesh_t* mesh;
    enemy_type_t type;
    enemy_state_t state;
    vec3_t target;      // shooter: where it parks
    float timer;        // time in the current state
    float life;         // total time alive
    float shot_timer;   // shooter: countdown to the next shot
    float cooldown;     // shooter: pause between bursts, scaled by difficulty
    int bursts_left;
    int shots_left;
} enemy_t;

int enemy_init(enemy_t* enemy, vec3_t pos, scene_t* scene);
void enemy_draw(enemy_t* enemy, renderer_t* renderer, camera_t camera, light_t* lights, int light_count, vec3_t ambient);
void enemy_destroy(enemy_t* enemy);

#endif
