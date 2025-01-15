idf_component_register(SRCS "app_timers.c"
                    INCLUDE_DIRS "include"
                    REQUIRES freertos
                    REQUIRES rtc
                    REQUIRES esp_driver_gptimer)

