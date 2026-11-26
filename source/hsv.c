#include <stdint.h>
#include "pwm.h"
#include "hsv.h"
#include "nrf_log.h"

void led2_set_hsv(uint16_t h, uint8_t s, uint8_t v)
{
  uint32_t value = v * 255 / 100;
  uint32_t sat = s * 255 / 100;
  uint32_t sector = (h % 360) / 60;
  uint32_t step = (h % 60) * 255 / 60;

  uint32_t low = value * (255 - sat) / 255;
  uint32_t down = value * (255 - sat * step / 255) / 255;
  uint32_t up = value * (255 - sat * (255 - step) / 255) / 255;

  uint32_t r, g, b;

  if (sector == 0)
  {
    r = value;
    g = up;
    b = low;
  }
  else if (sector == 1)
  {
    r = down;
    g = value;
    b = low;
  }
  else if (sector == 2)
  {
    r = low;
    g = value;
    b = up;
  }
  else if (sector == 3)
  {
    r = low;
    g = down;
    b = value;
  }
  else if (sector == 4)
  {
    r = up;
    g = low;
    b = value;
  }
  else
  {
    r = value;
    g = low;
    b = down;
  }

  NRF_LOG_INFO("h:%d s:%d v:%d", h, s, v);

  pwm_set_rgb(r, g, b);
}