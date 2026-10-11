#include <math.h>
#include "math/mat4.h"

mat4_t mat4_identity(void) {
    return (mat4_t){{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};
}

mat4_t mat4_zero(void) {
    return (mat4_t){{
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    }};
}

mat4_t mat4_add(mat4_t a, mat4_t b) {
    mat4_t res;
    for (int i = 0; i < 16; i++) {
        res.m[i] = a.m[i] + b.m[i];
    }
    return res;
}

mat4_t mat4_sub(mat4_t a, mat4_t b) {
    mat4_t res;
    for (int i = 0; i < 16; i++) {
        res.m[i] = a.m[i] - b.m[i];
    }
    return res;
}

mat4_t mat4_mul(mat4_t a, mat4_t b) {
    mat4_t res = mat4_zero();
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            res.m[col * 4 + row] = sum;
        }
    }
    return res;
}

mat4_t mat4_transpose(mat4_t m) {
    mat4_t res;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            res.m[col * 4 + row] = m.m[row * 4 + col];
        }
    }
    return res;
}
