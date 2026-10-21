#include <string.h>
#include <SDL3/SDL.h>
#include "engine/input.h"

static bool curr[SDL_SCANCODE_COUNT];
static bool prev[SDL_SCANCODE_COUNT];

void input_update(void) {
    memcpy(prev, curr, sizeof(curr));

    int count;
    const bool* k = SDL_GetKeyboardState(&count);
    memcpy(curr, k, (size_t)count < sizeof(curr) ? (size_t)count : sizeof(curr));
}

int key_down(SDL_Scancode sc) {
    return curr[sc];
}

int key_pressed(SDL_Scancode sc) {
    return curr[sc] && !prev[sc];
}

int key_released(SDL_Scancode sc) {
    return !curr[sc] && prev[sc];
}
