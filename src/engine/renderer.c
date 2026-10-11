#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "engine/renderer.h"

#define CURSOR_RESET "\033[H"
#define CURSOR_HIDE "\033[?25l"
#define CURSOR_SHOW "\033[?25h"
#define SCREEN_CLEAR "\033[2J"

int renderer_init(renderer_t* renderer, int width, int height) {
    if(!renderer || width <= 0 || height <= 0) return 1;
    
    renderer->width = width;
    renderer->height = height;
    renderer->screen_buffer = malloc(width * height);

    if(!renderer->screen_buffer) return 1;
    
    printf(SCREEN_CLEAR CURSOR_RESET CURSOR_HIDE);
    renderer_clear(renderer);
    return 0;
}

void renderer_draw(renderer_t* renderer) {
    if(!renderer || !renderer->screen_buffer) return;
    
    printf(CURSOR_RESET);
    for(int y = 0; y < renderer->height; y++) {
        for(int x = 0; x < renderer->width; x++) {
            putchar(renderer->screen_buffer[y * renderer->width + x]);
        }
        putchar('\n');
    }
}

void renderer_clear(renderer_t* renderer) {
    if (!renderer || !renderer->screen_buffer) return;

    for (int i = 0; i < renderer->width * renderer->height; i++) {
        renderer->screen_buffer[i] = ' ';
    }
}

void renderer_draw_char(renderer_t* renderer, int x, int y, char c) {
    if (!renderer || !renderer->screen_buffer) return;

    if (x >= 0 && x < renderer->width && y >= 0 && y < renderer->height) {
        renderer->screen_buffer[y * renderer->width + x] = c;
    }
}

void renderer_destroy(renderer_t* renderer) {
    if(!renderer) return;
    free(renderer->screen_buffer);
    printf(CURSOR_SHOW);
}
