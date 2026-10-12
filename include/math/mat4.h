#ifndef MAT4_H
#define MAT4_H

#include "math/vec4.h"
#include "math/vec3.h"

typedef struct {
    float m[16];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_zero(void);
mat4_t mat4_add(mat4_t a, mat4_t b);
mat4_t mat4_sub(mat4_t a, mat4_t b);
mat4_t mat4_mul(mat4_t a, mat4_t b);
mat4_t mat4_transpose(mat4_t m);
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);

mat4_t mat4_translate(vec3_t t);
mat4_t mat4_scale(vec3_t s);
mat4_t mat4_rotx(float angle);
mat4_t mat4_roty(float angle);
mat4_t mat4_rotz(float angle);
mat4_t mat4_perspective(float fov, float aspect, float near, float far);
mat4_t mat4_lookat(vec3_t eye, vec3_t target);

#endif
