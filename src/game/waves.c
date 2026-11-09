#include <stdlib.h>
#include <math.h>
#include "game/waves.h"
#include "game/scene.h"
#include "game/enemy_ai.h"
#include "game/tuning.h"

typedef struct { enemy_type_t type; int count; float spacing; } wave_t;

static const wave_t wave_table[] = WAVE_TABLE;
#define WAVE_COUNT ((int)(sizeof(wave_table) / sizeof(wave_table[0])))

static float random_range(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

// Point on the camera ray through lane (a point on the y=0 plane) at depth y.
static vec3_t on_ray(vec3_t lane, float y, float cam_y) {
    float k = (y - cam_y) / -cam_y;
    return (vec3_t){ lane.x * k, y, lane.z * k };
}

// Lane in the central area, as far as possible from the lanes of enemies still coming in.
static vec3_t pick_lane(scene_t* scene, float cam_y) {
    vec3_t lane = { 0, 0, 0 };
    float best = -1.0f;

    for(int t = 0; t < SPAWN_TRIES && best < LANE_MIN_SEP; t++) {
        vec3_t c = {
            random_range(-SPAWN_CENTER_X, SPAWN_CENTER_X), 0,
            random_range(-SPAWN_CENTER_Z, SPAWN_CENTER_Z),
        };
        float d = 1e9f;
        for(int i = 0; i < scene->enemy_count; i++) {
            enemy_t* e = &scene->enemies[i];
            if(e->state >= ENEMY_LEAVING) continue;
            float k = -cam_y / (e->target.y - cam_y);   // target sits on the enemy's ray: scale it back to its lane
            d = fminf(d, vec3_dist(c, (vec3_t){ e->target.x * k, 0, e->target.z * k }));
        }
        if(d > best) { best = d; lane = c; }
    }
    return lane;
}

static void spawn(scene_t* scene, enemy_type_t type, int level) {
    // the enemy flies down the camera ray through its lane, so it stays put on screen and grows
    float cam_y = scene->camera.pos.y;
    vec3_t lane = pick_lane(scene, cam_y);
    vec3_t target = on_ray(lane, type == ENEMY_SHOOTER ? SHOOTER_HOLD_Y : 0.0f, cam_y);
    vec3_t pos = on_ray(lane, SPAWN_Y, cam_y);
    if(vec3_dist(pos, scene->player.pos) < SPAWN_SAFE_DIST) pos.y = scene->player.pos.y + SPAWN_SAFE_DIST;

    if(spawn_enemy(scene, pos) != 0) return;

    float speed_mul = fminf(1.0f + DIFF_SPEED_STEP * level, DIFF_SPEED_MAX);
    float cooldown_mul = fmaxf(1.0f / (1.0f + DIFF_COOLDOWN_STEP * level), DIFF_COOLDOWN_MIN);
    enemy_t* e = &scene->enemies[scene->enemy_count - 1];
    enemy_ai_setup(e, type, speed_mul, cooldown_mul);
    e->target = target;
    e->vel = vec3_scale(vec3_normalize(vec3_sub(target, pos)), e->spd);
}

// Endless run: spawn the current wave, wait for the screen to clear, pause, start the next one harder.
void waves_update(scene_t* scene, float dt) {
    waves_t* w = &scene->waves;
    int level = w->started > 0 ? w->started - 1 : 0;
    const wave_t* cur = &wave_table[level % WAVE_COUNT];

    if(w->left > 0) {
        if((w->timer -= dt) > 0) return;
        spawn(scene, cur->type, level);
        w->left--;
        w->timer = cur->spacing;
        return;
    }
    if(scene->enemy_count > 0) { w->timer = WAVE_GAP; return; }
    if((w->timer -= dt) > 0) return;

    level = w->started++;
    int extra = level / DIFF_GROW_EVERY;
    int count = wave_table[level % WAVE_COUNT].count + (extra < DIFF_GROW_MAX ? extra : DIFF_GROW_MAX);
    w->left = count < MAX_ENEMIES ? count : MAX_ENEMIES;
    w->timer = 0;
}
