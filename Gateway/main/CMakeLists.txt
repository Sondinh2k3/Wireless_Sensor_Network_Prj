# Register the main component
idf_component_register(SRCS "app_main.c"
                            "../common/dht11/dht11.c"
                            "../common/http_server_app/http_server_app.c"
                            "../common/sx1278_ra02/ra02.c"
                    INCLUDE_DIRS "."
                    "../common/dht11"
                    "../common/http_server_app"
                    "../common/sx1278_ra02")
