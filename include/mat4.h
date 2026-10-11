#ifndef MAT4_H
#define MAT4_H

typedef struct {
    float m[16];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_zero(void);
mat4_t mat4_add(mat4_t a, mat4_t b);
mat4_t mat4_sub(mat4_t a, mat4_t b);
mat4_t mat4_mul(mat4_t a, mat4_t b);
mat4_t mat4_transpose(mat4_t m);

#endif