#ifndef CAMERA_H
#define CAMERA_H

#include "math/vec3.h"

typedef struct {
    vec3_t pos;
    vec3_t target;
    float fov;
    float near;
    float far;
} camera_t;

#endif
