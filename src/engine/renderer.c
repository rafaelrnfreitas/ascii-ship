#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "engine/renderer.h"
#include "graphics/mesh.h"

#define CURSOR_RESET "\033[H"
#define CURSOR_HIDE "\033[?25l"
#define CURSOR_SHOW "\033[?25h"
#define SCREEN_CLEAR "\033[2J"

int renderer_init(renderer_t* renderer, int width, int height) {
    if(!renderer || width <= 0 || height <= 0) return 1;
    
    renderer->width = width;
    renderer->height = height;
    renderer->screen_buffer = malloc((size_t)width * (size_t)height);

    if(!renderer->screen_buffer) return 1;

    fputs(SCREEN_CLEAR CURSOR_RESET CURSOR_HIDE, stdout);
    fflush(stdout);
    renderer_clear(renderer);
    return 0;
}

void renderer_draw(renderer_t* renderer) {
    if(!renderer || !renderer->screen_buffer) return;

    size_t w = (size_t)renderer->width;
    size_t h = (size_t)renderer->height;
    size_t cap = sizeof(CURSOR_RESET) + h * (w + 2);

    char* frame = malloc(cap);
    if(!frame) return;

    size_t n = 0;
    memcpy(frame, CURSOR_RESET, sizeof(CURSOR_RESET) - 1);
    n += sizeof(CURSOR_RESET) - 1;

    for(size_t y = 0; y < h; y++) {
        memcpy(frame + n, renderer->screen_buffer + y * w, w);
        n += w;
        if(y + 1 < h) {
            frame[n++] = '\r';
            frame[n++] = '\n';
        }
    }

    fwrite(frame, 1, n, stdout);
    fflush(stdout);
    free(frame);
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

vec3_t proj_vert(vertex_t v, mat4_t mvp, int width, int height) {
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

void renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh, mat4_t mvp) {
    if(!renderer || !mesh) return;

    for(int i = 0; i < mesh->index_count / 3; i++) {
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
        
        renderer_draw_char(renderer, p1.x, p1.y, '*');
        renderer_draw_char(renderer, p2.x, p2.y, '*');
        renderer_draw_char(renderer, p3.x, p3.y, '*');
    }
}

void renderer_destroy(renderer_t* renderer) {
    if(!renderer) return;

    free(renderer->screen_buffer);
    renderer->screen_buffer = NULL;

    fputs(CURSOR_SHOW, stdout);
    fflush(stdout);
}
