#ifndef ENGINE_H
#define ENGINE_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

typedef struct {
    int running;
    char screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
} engine_t;

int engine_init(engine_t* engine);
void engine_update(engine_t* engine, float dt);
void engine_draw(engine_t* engine);
void engine_draw_char(engine_t* engine, int x, int y, char c);
void engine_run(engine_t* engine);
void engine_shutdown(engine_t* engine);

#endif
