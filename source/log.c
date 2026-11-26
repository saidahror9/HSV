#include "nrf_log_ctrl.h"
#include "log.h"
#include "app_error.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

void logs_init(void)
{
  ret_code_t ret = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(ret);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void logs_process(void)
{
  while (NRF_LOG_PROCESS())
  {
  }
  LOG_BACKEND_USB_PROCESS();
}