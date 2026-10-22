#ifndef SHIP_H
#define SHIP_H

#include <stdint.h>
#include "graphics/mesh.h"

typedef struct {
    uint32_t body;
    uint32_t detail;
    uint32_t engine;
    uint32_t window;
} ship_colors_t;

mesh_t* ship_mesh_create(ship_colors_t palette);

#endif
