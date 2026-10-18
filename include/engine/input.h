#ifndef INPUT_H
#define INPUT_H

typedef enum {
    INPUT_KEY_RIGHT,
    INPUT_KEY_LEFT,
    INPUT_KEY_UP,
    INPUT_KEY_DOWN,
    INPUT_KEY_W,
    INPUT_KEY_A,
    INPUT_KEY_S,
    INPUT_KEY_D,
    INPUT_KEY_SPACE,
    INPUT_KEY_ENTER,
    INPUT_KEY_ESC_QUIT,
    INPUT_KEY_COUNT
} input_key_t;

#ifndef INPUT_HOLD_MS
#define INPUT_HOLD_MS 120
#endif

int input_init(void);
void input_update(void);
void input_shutdown(void);
int input_check_pressed(input_key_t key);
int input_check(input_key_t key);
int input_check_released(input_key_t key);

#endif