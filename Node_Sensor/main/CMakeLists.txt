# Register the main component
idf_component_register(SRCS "app_main.c"
                            "../common/DHT22/DHT22.c"
                            "../common/sx1278_ra02/ra02.c"
                    INCLUDE_DIRS "."
                    "../common/DHT22"
                    "../common/sx1278_ra02")
