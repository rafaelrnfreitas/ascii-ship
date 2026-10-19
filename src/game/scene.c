#include "game/scene.h"
#include "graphics/mesh.h"
#include "math/mat4.h"

static float angle = 0;

int scene_init(scene_t* scene) {
    if(!scene) return 1;

    scene->mesh_count = 0;
    scene->camera = (camera_t){{10, 0, 0}, {0, 0, 0}};

    vertex_t verts[] = {
        {{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f,  1.0f}},

        {{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},

        {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},

        {{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}},

        {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},

        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}}
    };

    int indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    mesh_t* cube = mesh_create(
        verts,
        sizeof(verts) / sizeof(verts[0]),
        indices,
        sizeof(indices) / sizeof(indices[0])
    );

    if(!cube) return 1;

    scene->meshes[scene->mesh_count++] = cube;
    return 0;
}

void scene_update(scene_t* scene, float dt) {
    angle += dt;
}

void scene_draw(scene_t* scene, renderer_t* renderer) {
    for(size_t i = 0; i < scene->mesh_count; i++) {
        mat4_t model = mat4_identity();   
        model = mat4_mul(mat4_rotx(angle), model);
        model = mat4_mul(mat4_roty(angle), model);
        model = mat4_mul(mat4_rotz(angle), model);

        renderer_draw_mesh(renderer, scene->meshes[i], model, scene->camera);
    }
}

void scene_destroy(scene_t* scene) {
    if(!scene) return;

    for(size_t i = 0; i < scene->mesh_count; i++) {
        mesh_destroy(scene->meshes[i]);
        scene->meshes[i] = NULL;
    }
    scene->mesh_count = 0;
}
