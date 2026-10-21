#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL_scancode.h>

void input_update(void);
int key_down(SDL_Scancode sc);
int key_pressed(SDL_Scancode sc);
int key_released(SDL_Scancode sc);

#endif
