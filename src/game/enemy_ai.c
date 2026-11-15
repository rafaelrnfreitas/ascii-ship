#include <math.h>
#include "game/enemy_ai.h"
#include "game/scene.h"
#include "game/tuning.h"
#include "graphics/color.h"

static void set_state(enemy_t* enemy, enemy_state_t state) {
    enemy->state = state;
    enemy->timer = 0;
}

void enemy_ai_setup(enemy_t* enemy, enemy_type_t type, float speed_mul, float cooldown_mul) {
    static const float speeds[] = { DRIFTER_SPEED, CHASER_SPEED, SHOOTER_SPEED };
    static const uint32_t colors[] = { COLOR_RED, COLOR_ORANGE, COLOR_MAGENTA };

    enemy->type = type;
    enemy->spd = speeds[type] * speed_mul;
    enemy->vel = (vec3_t){ 0, -enemy->spd, 0 };
    enemy->target = (vec3_t){ enemy->pos.x, HOLD_Y, enemy->pos.z };
    enemy->cooldown = BURST_COOLDOWN * cooldown_mul;
    enemy->bursts_left = BURSTS;
    enemy->shots_left = 0;
    enemy->shot_timer = 0;
    enemy->life = 0;
    set_state(enemy, ENEMY_ENTERING);

    for(size_t i = 0; i < enemy->mesh->index_count / 3; i++)
        if(enemy->mesh->colors[i] == COLOR_RED) enemy->mesh->colors[i] = colors[type];
}

void enemy_ai_kill(enemy_t* enemy) {
    set_state(enemy, ENEMY_DYING);
    enemy->vel = vec3_scale(enemy->vel, 0.3f);
}

int enemy_ai_visible(const enemy_t* enemy) {
    int telegraph = enemy->state == ENEMY_ACTIVE && enemy->shots_left == 0
        && enemy->shot_timer > 0 && enemy->shot_timer <= TELEGRAPH_TIME;
    if(telegraph) return (int)(enemy->shot_timer / ENEMY_BLINK) % 2 == 0;
    return enemy->state != ENEMY_DYING || (int)(enemy->timer / ENEMY_BLINK) % 2 == 0;
}

static void fire(enemy_t* enemy, scene_t* scene) {
    enemy_shot_fire(scene, enemy->pos, vec3_sub(scene->player.pos, enemy->pos));
}

static void active(enemy_t* enemy, scene_t* scene, float dt) {
    enemy->shot_timer -= dt;
    if(enemy->shot_timer > 0) return;

    if(enemy->shots_left == 0) {
        enemy->shots_left = BURST_SHOTS;
        enemy->shot_timer = 0;
        return;
    }
    fire(enemy, scene);
    enemy->shot_timer = SHOT_GAP;
    if(--enemy->shots_left == 0) {
        enemy->shot_timer = enemy->cooldown;
        if(--enemy->bursts_left == 0) set_state(enemy, ENEMY_LEAVING);
    }
}

int enemy_ai_update(enemy_t* enemy, scene_t* scene, float dt) {
    enemy->timer += dt;
    enemy->life += dt;

    if(enemy->life > ENEMY_MAX_LIFE && enemy->state < ENEMY_LEAVING) set_state(enemy, ENEMY_LEAVING);

    switch(enemy->state) {
    case ENEMY_ENTERING: {
        vec3_t to = vec3_sub(enemy->target, enemy->pos);
        if(vec3_len(to) <= enemy->spd * dt) {
            enemy->pos = enemy->target;
            enemy->vel = (vec3_t){ 0, 0, 0 };
            set_state(enemy, ENEMY_ACTIVE);
            enemy->shot_timer = enemy->cooldown;
            return 0;
        }
        enemy->vel = vec3_scale(vec3_normalize(to), enemy->spd);
        break;
    }
    case ENEMY_ACTIVE:
        active(enemy, scene, dt);
        break;
    case ENEMY_LEAVING:
        enemy->vel = (vec3_t){ enemy->pos.x < 0 ? -enemy->spd : enemy->spd, 0, enemy->spd * 0.5f };
        break;
    case ENEMY_DYING:
        if(enemy->timer >= DYING_TIME) return 1;
        break;
    }

    enemy->pos = vec3_add(enemy->pos, vec3_scale(enemy->vel, dt));

    return enemy->pos.y < DESPAWN_Y || enemy->pos.y > SPAWN_Y + 20.0f
        || fabsf(enemy->pos.x) > BOUNDS_X || fabsf(enemy->pos.z) > BOUNDS_Z;
}
