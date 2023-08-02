idf_component_register(SRCS "leds.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                    REQUIRES espressif__led_strip)

