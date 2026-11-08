#include <math.h>
#include "game/enemy_ai.h"
#include "game/scene.h"
#include "game/tuning.h"
#include "graphics/color.h"

static void set_state(enemy_t* enemy, enemy_state_t state) {
    enemy->state = state;
    enemy->timer = 0;
}

// Call right after enemy_init: picks the type, its speed and its color.
void enemy_ai_setup(enemy_t* enemy, enemy_type_t type, float speed_mul, float cooldown_mul) {
    static const float speeds[] = { DRIFTER_SPEED, CHASER_SPEED, SHOOTER_SPEED };
    static const uint32_t colors[] = { COLOR_RED, COLOR_ORANGE, COLOR_MAGENTA };

    enemy->type = type;
    enemy->spd = speeds[type] * speed_mul;
    enemy->vel = (vec3_t){ 0, -enemy->spd, 0 };
    enemy->target = (vec3_t){ enemy->pos.x, SHOOTER_HOLD_Y, enemy->pos.z };
    enemy->cooldown = SHOOTER_COOLDOWN * cooldown_mul;
    enemy->bursts_left = SHOOTER_BURSTS;
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
    int telegraph = enemy->type == ENEMY_SHOOTER && enemy->state == ENEMY_ACTIVE && enemy->shots_left == 0
        && enemy->shot_timer > 0 && enemy->shot_timer <= TELEGRAPH_TIME;
    if(telegraph) return (int)(enemy->shot_timer / ENEMY_BLINK) % 2 == 0;
    return enemy->state != ENEMY_DYING || (int)(enemy->timer / ENEMY_BLINK) % 2 == 0;
}

// Rotate dir toward want by at most max_angle radians.
static vec3_t turn_toward(vec3_t dir, vec3_t want, float max_angle) {
    float angle = acosf(fmaxf(-1.0f, fminf(1.0f, vec3_dot(dir, want))));
    if(angle <= max_angle) return want;
    float t = max_angle / angle;
    return vec3_normalize(vec3_add(vec3_scale(dir, 1.0f - t), vec3_scale(want, t)));
}

static void active(enemy_t* enemy, scene_t* scene, float dt) {
    vec3_t player = scene->player.pos;

    switch(enemy->type) {
    case ENEMY_DRIFTER:
        enemy->pos.x += DRIFTER_SWAY * cosf(enemy->life * DRIFTER_SWAY_FREQ) * dt;   // sway on top of the heading
        break;
    case ENEMY_CHASER:
        if(enemy->pos.y > player.y + CHASER_COMMIT_DIST) {
            vec3_t dir = vec3_normalize(enemy->vel);
            vec3_t want = vec3_normalize(vec3_sub(player, enemy->pos));
            enemy->vel = vec3_scale(turn_toward(dir, want, CHASER_TURN * dt), enemy->spd);
        }
        break;
    case ENEMY_SHOOTER:
        enemy->shot_timer -= dt;
        if(enemy->shot_timer <= 0) {
            if(enemy->shots_left == 0) {
                enemy->shots_left = SHOOTER_BURST;
                enemy->shot_timer = 0;
            } else {
                enemy_shot_fire(scene, enemy->pos);
                enemy->shot_timer = SHOOTER_SHOT_GAP;
                if(--enemy->shots_left == 0) {
                    enemy->shot_timer = enemy->cooldown;
                    if(--enemy->bursts_left == 0) set_state(enemy, ENEMY_LEAVING);
                }
            }
        }
        return;
    }

    // drifters and chasers leave once they have flown past the player
    if(enemy->pos.y < player.y - 4.0f) set_state(enemy, ENEMY_LEAVING);
}

// Returns 1 when the enemy should be removed.
int enemy_ai_update(enemy_t* enemy, scene_t* scene, float dt) {
    enemy->timer += dt;
    enemy->life += dt;

    if(enemy->life > ENEMY_MAX_LIFE && enemy->state < ENEMY_LEAVING) set_state(enemy, ENEMY_LEAVING);

    switch(enemy->state) {
    case ENEMY_ENTERING:
        if(enemy->type == ENEMY_SHOOTER) {
            vec3_t to = vec3_sub(enemy->target, enemy->pos);
            if(vec3_len(to) <= enemy->spd * dt) {
                enemy->pos = enemy->target;
                enemy->vel = (vec3_t){ 0, 0, 0 };
                set_state(enemy, ENEMY_ACTIVE);
                enemy->shot_timer = enemy->cooldown;
                return 0;
            }
            enemy->vel = vec3_scale(vec3_normalize(to), enemy->spd);
        } else if(enemy->pos.y < ENTER_Y) {
            set_state(enemy, ENEMY_ACTIVE);
        }
        break;
    case ENEMY_ACTIVE:
        active(enemy, scene, dt);
        break;
    case ENEMY_LEAVING:
        // parked shooters peel off sideways and up; the rest keep their heading
        if(enemy->type == ENEMY_SHOOTER)
            enemy->vel = (vec3_t){ enemy->pos.x < 0 ? -enemy->spd : enemy->spd, 0, enemy->spd * 0.5f };
        else {
            // peel off toward the nearest screen edge instead of flying into the camera
            vec3_t out = { enemy->pos.x, 0, enemy->pos.z };
            if(vec3_len(out) < 0.1f) out.x = 1;
            enemy->vel = vec3_add(vec3_scale(vec3_normalize(out), enemy->spd * 1.5f), (vec3_t){ 0, -enemy->spd * 0.5f, 0 });
        }
        break;
    case ENEMY_DYING:
        if(enemy->timer >= DYING_TIME) return 1;
        break;
    }

    enemy->pos = vec3_add(enemy->pos, vec3_scale(enemy->vel, dt));

    return enemy->pos.y < DESPAWN_Y || enemy->pos.y > SPAWN_Y + 20.0f
        || fabsf(enemy->pos.x) > BOUNDS_X || fabsf(enemy->pos.z) > BOUNDS_Z;
}
