#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "game/enemy.h"

void enemy_ai_setup(enemy_t* enemy, enemy_type_t type, float speed_mul, float cooldown_mul);
int enemy_ai_update(enemy_t* enemy, scene_t* scene, float dt);
void enemy_ai_kill(enemy_t* enemy);
int enemy_ai_visible(const enemy_t* enemy);

#endif
