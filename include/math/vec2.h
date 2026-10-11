#ifndef VEC2_H
#define VEC2_H

typedef struct {
    float x;
    float y;
} vec2_t;

vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t a, vec2_t b);
vec2_t vec2_div(vec2_t a, vec2_t b);
float vec2_dot(vec2_t a, vec2_t b);
float vec2_cross(vec2_t a, vec2_t b);
float vec2_len(vec2_t v);
vec2_t vec2_normalize(vec2_t v);
float vec2_len_sq(vec2_t v);

#endif
