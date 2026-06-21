idf_component_register(SRCS "apptask.c"
                    INCLUDE_DIRS "include"
                    REQUIRES freertos
                    REQUIRES leds
                    REQUIRES driver
                    REQUIRES esp_driver_gpio
                    REQUIRES sense
                    REQUIRES I2C
                    REQUIRES blufi
                    REQUIRES console
                    REQUIRES bt
                    REQUIRES rtc
                    REQUIRES app_timers
                    REQUIRES esp_driver_gptimer
                    REQUIRES esp_system)

