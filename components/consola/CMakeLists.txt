idf_component_register(SRCS "consola.c"
                    INCLUDE_DIRS "include"
                    REQUIRES freertos
                    REQUIRES sense
                    REQUIRES esp_wifi
                    REQUIRES blufi
                    REQUIRES rtc
                    REQUIRES init
                    REQUIRES app_timers
                    REQUIRES console
                    )

