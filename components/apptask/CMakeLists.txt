idf_component_register(SRCS "apptask.c"
                    INCLUDE_DIRS "include"
                    REQUIRES freertos
                    REQUIRES leds
                    REQUIRES sense
                    REQUIRES I2C
                    REQUIRES blufi
                    REQUIRES console
                    REQUIRES rtc
                    REQUIRES esp_system)

