#ifndef VEC4_H
#define VEC4_H

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

vec4_t vec4_add(vec4_t a, vec4_t b);
vec4_t vec4_sub(vec4_t a, vec4_t b);
vec4_t vec4_mul(vec4_t a, vec4_t b);
vec4_t vec4_div(vec4_t a, vec4_t b);
float vec4_dot(vec4_t a, vec4_t b);
float vec4_len(vec4_t v);
vec4_t vec4_normalize(vec4_t v);
float vec4_len_sq(vec4_t v);

#endif
