#include <stdint.h>
#include <stdbool.h>
#include "nrf_delay.h"
#include "app_timer.h"
#include "app_error.h"
#include "nrf_log.h"
#include "log.h"
#include "pwm.h"
#include "hsv.h"
#include "button.h"

#define DEFAULT_HUE (98 * 360 / 100)

// int id[4] = {7, 1, 9, 8}

#define MODE_NONE 0
#define MODE_HUE 1
#define MODE_SAT 2
#define MODE_VAL 3

#define SLOW_BLINK_MS 500
#define FAST_BLINK_MS 100

APP_TIMER_DEF(blink_timer);

int mode = MODE_NONE;
int hue = DEFAULT_HUE;
int sat = 100;
int val = 100;
int sat_step = -1;
int val_step = -1;
bool led1_on = false;

void blink_done(void *p_context)
{
    led1_on = !led1_on;
    if (led1_on)
    {
        pwm_set_led1(255);
    }
    else
    {
        pwm_set_led1(0);
    }
}

void led1_show_mode(void)
{
    app_timer_stop(blink_timer);
    led1_on = false;

    if (mode == MODE_NONE)
    {
        pwm_set_led1(0);
    }
    else if (mode == MODE_HUE)
    {
        app_timer_start(blink_timer, APP_TIMER_TICKS(SLOW_BLINK_MS), NULL);
    }
    else if (mode == MODE_SAT)
    {
        app_timer_start(blink_timer, APP_TIMER_TICKS(FAST_BLINK_MS), NULL);
    }
    else
    {
        pwm_set_led1(255);
    }
}

void button_double_click(void)
{
    mode++;
    if (mode > MODE_VAL)
    {
        mode = MODE_NONE;
    }
    NRF_LOG_INFO("mode %d", mode);
    led1_show_mode();
}

void button_hold_tick(void)
{
    if (mode == MODE_HUE)
    {
        hue++;
        if (hue >= 360)
        {
            hue = 0;
        }
    }
    else if (mode == MODE_SAT)
    {
        sat = sat + sat_step;
        if (sat >= 100)
        {
            sat = 100;
            sat_step = -1;
        }
        if (sat <= 0)
        {
            sat = 0;
            sat_step = 1;
        }
    }
    else if (mode == MODE_VAL)
    {
        val = val + val_step;
        if (val >= 100)
        {
            val = 100;
            val_step = -1;
        }
        if (val <= 0)
        {
            val = 0;
            val_step = 1;
        }
    }

    led2_set_hsv(hue, sat, val);
}

int main(void)
{
    logs_init();

    ret_code_t err = app_timer_init();
    APP_ERROR_CHECK(err);
    err = app_timer_create(&blink_timer, APP_TIMER_MODE_REPEATED, blink_done);
    APP_ERROR_CHECK(err);

    pwm_init();
    led2_set_hsv(hue, sat, val);
    led1_show_mode();

    button_init();

    NRF_LOG_INFO("Началась");

    while (true)
    {
        logs_process();
        nrf_delay_ms(1);
    }
}
