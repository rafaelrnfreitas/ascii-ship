#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>
#include "math/vec3.h"

#define MAX_LIGHTS 32

typedef enum {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOT
} light_type_t;

typedef struct {
    light_type_t type;
    uint32_t color;
    float intensity;

    union {
        struct {
            vec3_t dir;
        } dir;

        struct {
            vec3_t pos; 
            float range;
        } point;
    };
} light_t;

light_t light_create_dir(vec3_t dir, uint32_t color, float intensity);
light_t light_create_point(vec3_t pos, float range, uint32_t color, float intensity);

#endif
