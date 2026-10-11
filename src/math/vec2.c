#include <math.h>
#include "math/vec2.h"

vec2_t vec2_add(vec2_t a, vec2_t b) {
    return (vec2_t){a.x + b.x, a.y + b.y};
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    return (vec2_t){a.x - b.x, a.y - b.y};
}

vec2_t vec2_mul(vec2_t a, vec2_t b) {
    return (vec2_t){a.x * b.x, a.y * b.y};
}

vec2_t vec2_div(vec2_t a, vec2_t b) {
    return (vec2_t){a.x / b.x, a.y / b.y};
}

float vec2_dot(vec2_t a, vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

float vec2_cross(vec2_t a, vec2_t b) {
    return a.x * b.y - a.y * b.x;
}

float vec2_len(vec2_t v) {
    return sqrtf(v.x * v.x + v.y * v.y); 
}

vec2_t vec2_normalize(vec2_t v) {
    float len = vec2_len(v);
    if(len == 0.0f) return (vec2_t){0.0f, 0.0f};
    return (vec2_t){v.x / len, v.y / len};
}

float vec2_len_sq(vec2_t v) {
    return v.x * v.x + v.y * v.y;
}

