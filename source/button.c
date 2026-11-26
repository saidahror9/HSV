#include <stdbool.h>
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "app_timer.h"
#include "app_error.h"
#include "button.h"

#define BUTTON_1 NRF_GPIO_PIN_MAP(1, 6)
#define DEBOUNCE_MS 20
#define DOUBLE_CLICK_MS 400
#define HOLD_DELAY_MS 300
#define HOLD_REPEAT_MS 30

APP_TIMER_DEF(debounce_timer);
APP_TIMER_DEF(double_click_timer);
APP_TIMER_DEF(hold_delay_timer);
APP_TIMER_DEF(hold_repeat_timer);

bool pressed = false;
bool holding = false;
int clicks = 0;

void debounce_done(void *p_context)
{
  bool now_pressed = (nrf_gpio_pin_read(BUTTON_1) == 0);

  if (now_pressed && !pressed)
  {
    pressed = true;
    app_timer_start(hold_delay_timer, APP_TIMER_TICKS(HOLD_DELAY_MS), NULL);
  }
  else if (!now_pressed && pressed)
  {
    pressed = false;
    app_timer_stop(hold_delay_timer);
    app_timer_stop(hold_repeat_timer);

    if (holding)
    {
      holding = false;
      clicks = 0;
    }
    else
    {
      clicks++;
      if (clicks == 1)
      {
        app_timer_start(double_click_timer, APP_TIMER_TICKS(DOUBLE_CLICK_MS), NULL);
      }
      else
      {
        app_timer_stop(double_click_timer);
        clicks = 0;
        button_double_click();
      }
    }
  }
}

void double_click_done(void *p_context)
{
  clicks = 0;
}

void hold_delay_done(void *p_context)
{
  if (pressed)
  {
    holding = true;
    app_timer_start(hold_repeat_timer, APP_TIMER_TICKS(HOLD_REPEAT_MS), NULL);
  }
}

void hold_repeat_done(void *p_context)
{
  button_hold_tick();
}

void button_callback(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
  app_timer_stop(debounce_timer);
  app_timer_start(debounce_timer, APP_TIMER_TICKS(DEBOUNCE_MS), NULL);
}

void button_init(void)
{
  ret_code_t err;

  err = app_timer_create(&debounce_timer, APP_TIMER_MODE_SINGLE_SHOT, debounce_done);
  APP_ERROR_CHECK(err);
  err = app_timer_create(&double_click_timer, APP_TIMER_MODE_SINGLE_SHOT, double_click_done);
  APP_ERROR_CHECK(err);
  err = app_timer_create(&hold_delay_timer, APP_TIMER_MODE_SINGLE_SHOT, hold_delay_done);
  APP_ERROR_CHECK(err);
  err = app_timer_create(&hold_repeat_timer, APP_TIMER_MODE_REPEATED, hold_repeat_done);
  APP_ERROR_CHECK(err);

  err = nrfx_gpiote_init();
  APP_ERROR_CHECK(err);

  nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
  config.pull = NRF_GPIO_PIN_PULLUP;

  err = nrfx_gpiote_in_init(BUTTON_1, &config, button_callback);
  APP_ERROR_CHECK(err);
  nrfx_gpiote_in_event_enable(BUTTON_1, true);
}