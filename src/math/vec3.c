#include <math.h>
#include "math/vec3.h"

vec3_t vec3_add(vec3_t a, vec3_t b) {
    return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3_t vec3_mul(vec3_t a, vec3_t b) {
    return (vec3_t){a.x * b.x, a.y * b.y, a.z * b.z};
}

vec3_t vec3_div(vec3_t a, vec3_t b) {
    return (vec3_t){a.x / b.x, a.y / b.y, a.z / b.z};
}

vec3_t vec3_negate(vec3_t v) {
    return (vec3_t){-v.x, -v.y, -v.z};
}

vec3_t vec3_scale(vec3_t v, float s) {
    return (vec3_t){v.x * s, v.y * s, v.z * s};
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return (vec3_t) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_len(vec3_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); 
}

vec3_t vec3_normalize(vec3_t v) {
    float len = vec3_len(v);
    if(len == 0.0f) return (vec3_t){0.0f, 0.0f};
    return (vec3_t){v.x / len, v.y / len, v.z / len};
}

float vec3_len_sq(vec3_t v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
