#include <stdlib.h>
#include <string.h>
#include "graphics/mesh.h"

mesh_t* mesh_create(
    const vertex_t* verts,
    size_t vert_count,
    const int* indices,
    size_t index_count,
    const uint32_t* colors
) {
    if(!verts || !indices || vert_count == 0 || index_count == 0) return NULL;

    mesh_t* mesh = calloc(1, sizeof(mesh_t)); 
    if(!mesh) return NULL;

    size_t verts_size = vert_count * sizeof(vertex_t);
    size_t indices_size = index_count * sizeof(int);

    mesh->verts = malloc(verts_size);
    if(!mesh->verts) {
        free(mesh);
        return NULL;
    }

    mesh->indices = malloc(indices_size);
    if(!mesh->indices) {
        free(mesh->verts);
        free(mesh);
        return NULL;
    }

    if(colors) {
        size_t colors_size = (index_count / 3) * sizeof(uint32_t);

        mesh->colors = malloc(colors_size);
        if(!mesh->colors) {
            free(mesh->indices);
            free(mesh->verts);
            free(mesh);
            return NULL;
        }

        memcpy(mesh->colors, colors, colors_size);
    }

    memcpy(mesh->verts, verts, verts_size);
    memcpy(mesh->indices, indices, indices_size);

    mesh->vert_count = vert_count;
    mesh->index_count = index_count;

    return mesh;
}

void mesh_destroy(mesh_t* mesh) {
    if(!mesh) return;

    free(mesh->verts);
    free(mesh->indices);
    free(mesh->colors);
    free(mesh);
}
