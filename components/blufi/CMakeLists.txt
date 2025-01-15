idf_component_register(SRCS "blufi.c"
                            "blufi_security.c"
                            "blufi_init.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                    
                    REQUIRES mbedtls
                    REQUIRES freertos
                    REQUIRES esp_wifi
                    REQUIRES nvs_flash
                    REQUIRES lwip
                    REQUIRES esp_netif
                    REQUIRES sense
                    REQUIRES apptask
                    REQUIRES bt)

