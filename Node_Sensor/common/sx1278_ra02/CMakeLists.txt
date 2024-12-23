set(app_src ra02.c)
set(pri_req driver esp_timer)
idf_component_register(SRCS ${app_src} 
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})