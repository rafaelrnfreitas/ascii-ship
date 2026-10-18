#include <stdio.h>
#include "engine/engine.h"

int main() {
    engine_t engine;

    if(engine_init(&engine) != 0) {
        fprintf(stderr, "failed to initialize engine\n");
        return 1;
    }

    engine_run(&engine);
    engine_shutdown(&engine);

    return 0;
}
