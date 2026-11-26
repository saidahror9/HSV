#include <stdint.h>

void pwm_init(void);
void pwm_set_led1(uint8_t brightness);
void pwm_set_rgb(uint8_t r, uint8_t g, uint8_t b);