#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <SDL3/SDL.h>
#include "graphics/mesh.h"
#include "graphics/camera.h"
#include "math/mat4.h"
#include "graphics/light.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* texture;
    uint32_t* pixels;
    float* depth_buffer;
    int width;
    int height;
} renderer_t;

int renderer_init(renderer_t* r, int width, int height, int win_w, int win_h);
void renderer_draw(renderer_t* r);
void renderer_clear(renderer_t* r);
void renderer_draw_pixel(renderer_t* r, int x, int y, uint32_t color); 
void renderer_draw_mesh(
    renderer_t* renderer, 
    mesh_t* mesh, 
    mat4_t model, 
    camera_t cam, 
    light_t* lights, 
    int light_count
);
void renderer_destroy(renderer_t* r);

#endif
