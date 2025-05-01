#pragma once

#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EXAMPLE_TOUCH_LSLEEP_WAKEUP_SUPPORT (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)

esp_err_t PwrMngr_register_timer_wakeup(void);

void PwrMngr_Init();

#ifdef __cplusplus
}
#endif