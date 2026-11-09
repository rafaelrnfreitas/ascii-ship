#ifndef WAVES_H
#define WAVES_H

typedef struct scene_s scene_t;

typedef struct {
    int started;   // waves started so far; the difficulty level is started - 1
    int left;      // enemies of the current wave still to spawn
    float timer;
} waves_t;

void waves_update(scene_t* scene, float dt);

#endif
