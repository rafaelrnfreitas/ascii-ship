#include "game/scene.h"
#include "graphics/mesh.h"
#include "math/mat4.h"

static float angle = 0;

int scene_init(scene_t* scene) {
    if(!scene) return 1;

    scene->mesh_count = 0;

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

        mat4_t view = mat4_lookat((vec3_t){10, 0, 0}, (vec3_t){0, 0, 0});
        mat4_t proj = mat4_perspective(
                        0.5f, 
                        (float)renderer->width / (float)renderer->height, 
                        1, 
                        100 
                    );

        mat4_t mvp = mat4_mul(proj, mat4_mul(view, model));
        renderer_draw_mesh(renderer, scene->meshes[i], mvp);
    }
}

void scene_destroy(scene_t* scene) {
    if(!scene) return;

    for(size_t i = 0; i < scene->mesh_count; i++)
        mesh_destroy(scene->meshes[i]);
    scene->mesh_count = 0;
}
