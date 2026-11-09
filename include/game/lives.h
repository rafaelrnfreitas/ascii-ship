#ifndef LIVES_H
#define LIVES_H

#include <SDL3/SDL_stdinc.h>

#define LIVES_START 5
#define LIVES_MAX 5
#define LIVES_INVULN_TIME 2.0f
#define LIVES_BLINK_TIME 0.1f

// event.user.code values for events of type lives_event_type; data1 holds the lives count as intptr_t
enum { LIVES_CHANGED, LIFE_LOST, GAME_OVER };

extern Uint32 lives_event_type;

typedef struct {
    int count;
    float invuln;
} lives_t;

int lives_register(void);
void lives_reset(lives_t* lives);
int lives_hit(lives_t* lives);
void lives_update(lives_t* lives, float dt);
int lives_visible(const lives_t* lives);

#endif
