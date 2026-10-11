#ifndef RENDERER_H
#define RENDERER_H

typedef struct {
    char* screen_buffer;
    int width;
    int height;
} renderer_t;

int renderer_init(renderer_t* renderer, int width, int height);
void renderer_draw(renderer_t* renderer);
void renderer_clear(renderer_t* renderer);
void renderer_draw_char(renderer_t* renderer, int x, int y, char c); 
void renderer_destroy(renderer_t* renderer);

#endif
