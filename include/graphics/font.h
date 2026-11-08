#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "engine/renderer.h"

void font_draw(renderer_t* r, int x, int y, const char* text, int scale, uint32_t color);
int font_width(const char* text, int scale);

#endif
