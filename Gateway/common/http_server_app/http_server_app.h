#ifndef HTTP_SERVER_APP_H
#define HTTP_SERVER_APP_H

// #include <stdint.h> //De dung duoc cac kieu uint8_t, uint16_t

typedef void (*http_post_callback_t)(char *data, int len);
typedef void (*http_get_callback_t)(void);

void start_webserver(void);
void stop_webserver(void);

void http_set_callback_dht22(void *cb);

void dht22_response(char *data, int len);

#endif