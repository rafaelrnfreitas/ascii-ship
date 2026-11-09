#include <stdint.h>
#include <SDL3/SDL_events.h>
#include "game/lives.h"

Uint32 lives_event_type = 0;

static void lives_push(int code, int count) {
    if(!lives_event_type) return;
    SDL_Event e;
    SDL_zero(e);
    e.type = lives_event_type;
    e.user.code = code;
    e.user.data1 = (void*)(intptr_t)count;
    SDL_PushEvent(&e);
}

int lives_register(void) {
    if(lives_event_type) return 0;
    lives_event_type = SDL_RegisterEvents(1);
    return lives_event_type ? 0 : 1;
}

void lives_reset(lives_t* lives) {
    lives->count = LIVES_START > LIVES_MAX ? LIVES_MAX : LIVES_START;
    lives->invuln = 0;
    lives_push(LIVES_CHANGED, lives->count);
}

// Returns 1 if a life was taken. Invulnerability starts at once, so later hits in the same frame are ignored.
int lives_hit(lives_t* lives) {
    if(lives->invuln > 0 || lives->count <= 0) return 0;

    lives->count--;
    lives->invuln = LIVES_INVULN_TIME;
    lives_push(LIVES_CHANGED, lives->count);
    lives_push(LIFE_LOST, lives->count);
    if(lives->count == 0) lives_push(GAME_OVER, 0);
    return 1;
}

void lives_update(lives_t* lives, float dt) {
    if(lives->invuln <= 0) return;
    lives->invuln -= dt;
    if(lives->invuln < 0) lives->invuln = 0;
}

int lives_visible(const lives_t* lives) {
    if(lives->invuln <= 0) return 1;
    return (int)((LIVES_INVULN_TIME - lives->invuln) / LIVES_BLINK_TIME) % 2 == 1;
}
