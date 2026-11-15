#ifndef WAVES_H
#define WAVES_H

typedef struct scene_s scene_t;

typedef struct {
    int started;
    int left;
    float timer;
} waves_t;

void waves_update(scene_t* scene, float dt);

#endif
