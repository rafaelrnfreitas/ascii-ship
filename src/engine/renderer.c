#include <stdio.h>
#include <time.h>
#include <string.h>
#include "engine/renderer.h"

void renderer_draw_char(renderer_t* renderer, int x, int y, char c) {
    if (!renderer) return;

    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        renderer->screen_buffer[y * SCREEN_WIDTH + x] = c;
    }
}

void renderer_clear(renderer_t* renderer) {
    if (!renderer) return;

    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        renderer->screen_buffer[i] = ' ';
    }
}
