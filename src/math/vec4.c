#include <math.h>
#include "math/vec4.h"

vec4_t vec4_add(vec4_t a, vec4_t b) {
    return (vec4_t){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

vec4_t vec4_sub(vec4_t a, vec4_t b) {
    return (vec4_t){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

vec4_t vec4_mul(vec4_t a, vec4_t b) {
    return (vec4_t){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

vec4_t vec4_div(vec4_t a, vec4_t b) {
    return (vec4_t){a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

float vec4_dot(vec4_t a, vec4_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float vec4_len(vec4_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); 
}

vec4_t vec4_normalize(vec4_t v) {
    float len = vec4_len(v);
    if(len == 0.0f) return (vec4_t){0.0f, 0.0f, 0.0f, 0.0f};
    return (vec4_t){v.x / len, v.y / len, v.z / len, v.w / len};
}

float vec4_len_sq(vec4_t v) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}
