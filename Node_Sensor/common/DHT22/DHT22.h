/*
    DHT22 temperature sensor driver
*/

#ifndef DHT_H_
#define DHT_H_

// #define DHT_OK 0
// #define DHT_CHECKSUM_ERROR -1
// #define DHT_TIMEOUT_ERROR -2

// Liet ke cac loi
typedef enum
{
    DHT_TIMEOUT_ERROR = -2,
    DHT_CHECKSUM_ERROR,
    DHT_OK,
} CHECK_ERROR;

// == function prototypes =======================================

void setDHTgpio(int gpio);
void errorHandler(int response);
int readDHT();
float getHumidity();
float getTemperature();
int getSignalLevel(int usTimeOut, bool state);

#endif