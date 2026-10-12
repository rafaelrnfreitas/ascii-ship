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

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
    return (vec4_t){
        m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12] * v.w,
        m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13] * v.w,
        m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w,
        m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w
    };
}

mat4_t mat4_translate(vec3_t t) {
    return (mat4_t){{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        t.x, t.y, t.z, 1
    }};
}

mat4_t mat4_scale(vec3_t s) {
    return (mat4_t){{
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1
    }};
}

mat4_t mat4_rotx(float a) {
    float c = cosf(a);
    float s = sinf(a);

    return (mat4_t){{
        1, 0, 0, 0,
        0, c, s, 0,
        0, -s, c, 0,
        0, 0, 0, 1
    }};
}

mat4_t mat4_roty(float a) {
    float c = cosf(a);
    float s = sinf(a);

    return (mat4_t){{
        c, 0, -s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1
    }};
}

mat4_t mat4_rotz(float a) {
    float c = cosf(a);
    float s = sinf(a);

    return (mat4_t){{
        c, s, 0, 0,
        -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }};
}

mat4_t mat4_perspective(float fov, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov * 0.5f);

    return (mat4_t){{
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2.0f * far * near) / (near - far), 0
    }};
}

mat4_t mat4_lookat(vec3_t eye, vec3_t target) {
    vec3_t world_up = {0, 0, 1};

    vec3_t forward = vec3_normalize(vec3_sub(target, eye));
    vec3_t right = vec3_normalize(vec3_cross(forward, world_up));
    vec3_t up = vec3_cross(right, forward);

    return (mat4_t){{
        right.x, up.x, -forward.x, 0,
        right.y, up.y, -forward.y, 0,
        right.z, up.z, -forward.z, 0,
        -vec3_dot(right, eye), -vec3_dot(up, eye), vec3_dot(forward, eye), 1
    }};
}
