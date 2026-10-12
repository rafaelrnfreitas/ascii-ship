#include "game/scene.h"
#include "graphics/mesh.h"

int scene_init(scene_t* scene) {
    if(!scene) return 1;

    scene->mesh_count = 0;

    vertex_t verts[] = {
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };

    int indices[] = {0, 1, 2};
    mesh_t* triangle = mesh_create(verts, 3, indices, 3);
    if(!triangle) return 1;

    scene->meshes[scene->mesh_count++] = triangle;
    return 0;
}

void scene_update(scene_t* scene, float dt) {
}

void scene_draw(scene_t* scene, renderer_t* renderer) {
    static int x = 0;
    static int y = 0;

    x++;
    y++;

    renderer_draw_char(renderer, x, y, '.');
}

void scene_destroy(scene_t* scene) {
    if(!scene) return;

    for(size_t i = 0; i < scene->mesh_count; i++)
        mesh_destroy(scene->meshes[i]);
    scene->mesh_count = 0;
}
