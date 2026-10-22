#ifndef MESH_H
#define MESH_H

#include <stddef.h>
#include <stdint.h>
#include "math/vec3.h"

typedef struct {
    vec3_t pos;
    vec3_t normal;
} vertex_t;

typedef struct {
    vertex_t* verts;
    size_t vert_count;
    int* indices;
    size_t index_count;
    uint32_t* colors;
} mesh_t;

mesh_t* mesh_create(
    const vertex_t* verts,
    size_t vert_count,
    const int* indices,
    size_t index_count,
    const uint32_t* colors
);

void mesh_destroy(mesh_t* mesh);

#endif
