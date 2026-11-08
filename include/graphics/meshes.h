#ifndef MESHES_H
#define MESHES_H

#include "graphics/mesh.h"

extern mesh_t* mesh_ship;
extern mesh_t* mesh_laser;
extern mesh_t* mesh_circle;

int meshes_init();
void meshes_free();

#endif

