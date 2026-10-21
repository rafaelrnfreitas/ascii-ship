#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/renderer.h"
#include "graphics/mesh.h"

#define COLOR_BLACK 0x000000FFU
#define COLOR_WHITE 0xFFFFFFFFU

int renderer_init(renderer_t* r, int width, int height) {
    if(!r || width <= 0 || height <= 0) return 1;

    memset(r, 0, sizeof(renderer_t));
    r->width = width;
    r->height = height;
    
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer("Game", width, height, SDL_WINDOW_RESIZABLE, &r->window, &r->sdl_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        renderer_destroy(r);
        return 1;
    }

    SDL_SetRenderLogicalPresentation(r->sdl_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    r->texture = SDL_CreateTexture(
        r->sdl_renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        width, 
        height
    );
    if(!r->texture) {
        SDL_Log("Couldn't create texture: %s", SDL_GetError());
        return 1;
    }

    r->pixels = malloc((size_t)width * (size_t)height * sizeof(uint32_t));
    if(!r->pixels) {
        renderer_destroy(r);
        return 1;
    }

    renderer_clear(r);
    return 0;
}

void renderer_draw(renderer_t* r) {
    if(!r || !r->pixels) return;
    
    SDL_UpdateTexture(r->texture, NULL, r->pixels, r->width * sizeof(uint32_t));
    SDL_SetRenderDrawColor(r->sdl_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); 
    SDL_RenderClear(r->sdl_renderer);
    SDL_RenderTexture(r->sdl_renderer, r->texture, NULL, NULL);
    SDL_RenderPresent(r->sdl_renderer);
}

void renderer_clear(renderer_t* r) {
    if(!r || !r->pixels) return;

    size_t count = (size_t)r->width * (size_t)r->height;
    for(size_t i = 0; i < count; i++) {
        r->pixels[i] = COLOR_BLACK;
    }
}

void renderer_draw_pixel(renderer_t* r, int x, int y, uint32_t color) {
    if(!r || !r->pixels) return;

    if(x >= 0 && x < r->width && y >= 0 && y < r->height) {
        r->pixels[y * r->width + x] = color;
    }
}

static vec3_t proj_vert(vertex_t v, mat4_t mvp, int width, int height) {
    vec4_t p = (vec4_t){v.pos.x, v.pos.y, v.pos.z, 1.0f};
    p = mat4_mul_vec4(mvp, p);

    p.x /= p.w;
    p.y /= p.w;
    p.z /= p.w;

    return (vec3_t){
        (p.x + 1.0f) / 2 * width,
        (1.0f - p.y) / 2 * height,
        p.z
    };
}

void renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh, mat4_t model, camera_t cam) {
    if(!renderer || !mesh) return;

    mat4_t view = mat4_lookat(cam.pos, cam.target);
    mat4_t proj = mat4_perspective(
                    0.5f, 
                    (float)renderer->width / (float)renderer->height, 
                    1, 
                    100 
                );
    mat4_t mvp = mat4_mul(proj, mat4_mul(view, model));

    for(size_t i = 0; i + 2 < mesh->index_count; i += 3) {
        size_t i1 = mesh->indices[i];
        size_t i2 = mesh->indices[i + 1];
        size_t i3 = mesh->indices[i + 2];

        vertex_t v1 = mesh->verts[i1];
        vertex_t v2 = mesh->verts[i2];
        vertex_t v3 = mesh->verts[i3];

        vec3_t p1 = proj_vert(v1, mvp, renderer->width, renderer->height);
        vec3_t p2 = proj_vert(v2, mvp, renderer->width, renderer->height);
        vec3_t p3 = proj_vert(v3, mvp, renderer->width, renderer->height);

        if(p1.z < 0 || p2.z < 0 || p3.z < 0) continue;
        if(p1.z > 1 || p2.z > 1 || p3.z > 1) continue;

        float area = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        if(area >= 0.0f) continue; // face de tras ou degenerada

        float fmin_x = p1.x < p2.x ? p1.x : p2.x;
        float fmax_x = p1.x > p2.x ? p1.x : p2.x;
        float fmin_y = p1.y < p2.y ? p1.y : p2.y;
        float fmax_y = p1.y > p2.y ? p1.y : p2.y;
        if(p3.x < fmin_x) fmin_x = p3.x;
        if(p3.x > fmax_x) fmax_x = p3.x;
        if(p3.y < fmin_y) fmin_y = p3.y;
        if(p3.y > fmax_y) fmax_y = p3.y;

        int min_x = fmin_x < 0 ? 0 : (int)fmin_x;
        int min_y = fmin_y < 0 ? 0 : (int)fmin_y;
        int max_x = fmax_x >= renderer->width ? renderer->width - 1 : (int)fmax_x;
        int max_y = fmax_y >= renderer->height ? renderer->height - 1 : (int)fmax_y;

        for(int y = min_y; y <= max_y; y++) {
            for(int x = min_x; x <= max_x; x++) {
                float px = x + 0.5f;
                float py = y + 0.5f;

                float w1 = (p3.x - p2.x) * (py - p2.y) - (p3.y - p2.y) * (px - p2.x);
                float w2 = (p1.x - p3.x) * (py - p3.y) - (p1.y - p3.y) * (px - p3.x);
                float w3 = (p2.x - p1.x) * (py - p1.y) - (p2.y - p1.y) * (px - p1.x);

                if(w1 > 0.0f || w2 > 0.0f || w3 > 0.0f) continue;

                renderer_draw_pixel(renderer, x, y, COLOR_WHITE);
            }
        }
    }
}

void renderer_destroy(renderer_t* r) {
    if(!r) return;
    free(r->pixels);
    if(r->texture) SDL_DestroyTexture(r->texture);
    if(r->sdl_renderer) SDL_DestroyRenderer(r->sdl_renderer);
    if(r->window) SDL_DestroyWindow(r->window);
    *r = (renderer_t){0};
    SDL_Quit();
}
