#ifndef RENDERER_H
#define RENDERER_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

typedef struct {
    char screen_buffer[SCREEN_HEIGHT*SCREEN_WIDTH];
} renderer_t;

void renderer_draw_char(renderer_t* renderer, int x, int y, char c); 
void renderer_clear(renderer_t* renderer);

#endif