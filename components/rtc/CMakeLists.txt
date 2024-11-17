idf_component_register(SRCS "rtc.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_system
                    REQUIRES esp_netif
                    REQUIRES esp_timer
                    REQUIRES blufi)

