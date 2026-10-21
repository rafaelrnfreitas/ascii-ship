#include "graphics/light.h"

light_t light_create_dir(vec3_t dir, uint32_t color, float intensity) {
    light_t light = (light_t){
        .type = LIGHT_DIRECTIONAL,
        .color = color,
        .intensity = intensity
    };

    light.dir.dir = dir;

    return light;
}

light_t light_create_point(vec3_t pos, float range, uint32_t color, float intensity) {
    light_t light = (light_t){
        .type = LIGHT_POINT,
        .color = color,
        .intensity = intensity
    };

    light.point.pos = pos;
    light.point.range = range;

    return light;
}
