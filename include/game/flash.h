#ifndef FLASH_H
#define FLASH_H

#define MAX_FLASHES 16

#include "math/vec3.h"
#include "engine/renderer.h"
#include "graphics/camera.h"
    
typedef struct {
    vec3_t pos;
    float time;
    float duration;
    float size;
    int active;
    mesh_t* mesh;
} flash_t;

void flash_init(flash_t* flash, vec3_t pos, float size, float duration);
void flash_update(flash_t* flash, float dt);
void flash_draw(flash_t* flash, renderer_t* renderer, camera_t camera);

#endif
