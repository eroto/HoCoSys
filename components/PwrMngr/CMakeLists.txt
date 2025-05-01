idf_component_register(SRCS "PwrMngr.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_timer
                    REQUIRES esp_hw_support
                    REQUIRES esp_driver_gptimer)

