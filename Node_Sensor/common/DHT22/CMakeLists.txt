set(app_src DHT22.c)
set(pri_req)
idf_component_register(SRCS ${app_src} 
                    INCLUDE_DIRS "."
                    REQUIRES driver ${pri_req})