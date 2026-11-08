#ifndef SCREENSHAKE_H
#define SCREENSHAKE_H

#include "graphics/camera.h"

typedef struct scene_s scene_t;

typedef struct {
    scene_t* scene;
    vec3_t offset;
    float magnitude;
    float time;
    float decay;
} screenshake_t;

void screenshake_init(screenshake_t* screenshake, scene_t* scene);
void screenshake_update(screenshake_t* screenshake, float dt);
void screenshake(screenshake_t* screenshake, float magnitude, float time, float decay);

#endif
