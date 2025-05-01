idf_component_register(SRCS "init.c"
                    INCLUDE_DIRS "include"
                    REQUIRES leds
                    REQUIRES apptask
                    REQUIRES sense
                    REQUIRES I2C
                    REQUIRES blufi
                    REQUIRES app_timers
                    REQUIRES consola
                    REQUIRES PwrMngr)

