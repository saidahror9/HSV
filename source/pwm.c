#include "nrfx_pwm.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "pwm.h"

nrfx_pwm_t pwm = NRFX_PWM_INSTANCE(0);

uint16_t pwm_values[4];

nrf_pwm_sequence_t pwm_seq = {{pwm_values}, 4, 0, 0};

void pwm_init(void)
{
  nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG;

  nrfx_err_t err = nrfx_pwm_init(&pwm, &config, NULL);
  APP_ERROR_CHECK(err);

  nrfx_pwm_simple_playback(&pwm, &pwm_seq, 1, NRFX_PWM_FLAG_LOOP);
}

void pwm_set_led1(uint8_t brightness)
{
  pwm_values[0] = brightness;
}

void pwm_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  pwm_values[1] = r;
  pwm_values[2] = g;
  pwm_values[3] = b;
}