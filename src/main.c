#include <stdio.h>
#include "engine/engine.h"

int main() {
    engine_t engine;
    engine_init(&engine);
    engine_run(&engine);
    engine_shutdown(&engine);

	return 0;
}
