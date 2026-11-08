#include <stdlib.h>
#include "game/screenshake.h"
#include "game/scene.h"

static float random_range(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

void screenshake_init(screenshake_t* screenshake, scene_t* scene) {
    screenshake->scene = scene;
    screenshake->magnitude = 0.0f;
    screenshake->decay = 0.0f;
    screenshake->time = 0.0f;
    screenshake->offset = (vec3_t){0.0f, 0.0f, 0.0f};
}

void screenshake_update(screenshake_t* screenshake, float dt) {
    camera_t* cam = &screenshake->scene->camera;

    cam->pos = vec3_sub(cam->pos, screenshake->offset);
    cam->target = vec3_sub(cam->target, screenshake->offset);
    screenshake->offset = (vec3_t){0, 0, 0};

    if(screenshake->time <= 0.0f || screenshake->magnitude <= 0.0f) return;

    screenshake->time -= dt;
    screenshake->magnitude -= screenshake->decay * dt;
    if(screenshake->magnitude < 0.0f) screenshake->magnitude = 0.0f;

    screenshake->offset = (vec3_t){
        random_range(-screenshake->magnitude, screenshake->magnitude),
        0.0f,
        random_range(-screenshake->magnitude, screenshake->magnitude)
    };

    cam->pos = vec3_add(cam->pos, screenshake->offset);
    cam->target = vec3_add(cam->target, screenshake->offset);
}

void screenshake(screenshake_t* screenshake, float magnitude, float time, float decay) {
    if(magnitude > screenshake->magnitude) screenshake->magnitude = magnitude;
    if(time > screenshake->time) screenshake->time = time;
    screenshake->decay = decay;
}
