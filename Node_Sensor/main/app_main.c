
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "DHT22.h"

#include <inttypes.h>
#include "ra02.h"

// =============================================================================================
static SemaphoreHandle_t xMutex;

static const char *TAG = "DHT";
static int ret;
char data[50];
// Task read Dht22 Data
void DHT_task(void *pvParameter)
{

    ESP_LOGI(TAG, "Starting DHT Task\n\n");

    while (1)
    {
        ret = readDHT();
        errorHandler(ret);

        if (xSemaphoreTake(xMutex, portMAX_DELAY))
        {
            sprintf(data, "Hum: %.1f Tmp: %.1f\n", getHumidity(), getTemperature());
            xSemaphoreGive(xMutex);
        }

        ESP_LOGI(TAG, "Hum: %.1f Tmp: %.1f\n", getHumidity(), getTemperature());

        // -- wait at least 2 sec before reading again ------------
        // The interval of whole process must be beyond 2 seconds !!
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

// Task transfer data using LoRa from Node Sensor to Gateway
void task_tx(void *pvParameters)
{
    ESP_LOGI(pcTaskGetName(NULL), "Start");
    uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
    while (1)
    {
        int send_len;
        if (xSemaphoreTake(xMutex, portMAX_DELAY))
        {
            send_len = snprintf((char *)buf, sizeof(buf), "%s", data);
            xSemaphoreGive(xMutex);

            if (send_len < 0)
            {
                ESP_LOGE(pcTaskGetName(NULL), "Error formatting buffer");
                continue;
            }
            if (send_len >= sizeof(buf))
            {
                ESP_LOGE(pcTaskGetName(NULL), "Buffer overflow");
                continue;
            }
        }
        else
        {
            ESP_LOGE(pcTaskGetName(NULL), "Failed to take mutex");
            continue;
        }

        lora_send_packet(buf, send_len);
        ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent...", send_len);

        int lost = lora_packet_lost();
        if (lost != 0)
        {
            ESP_LOGW(pcTaskGetName(NULL), "%d packets lost", lost);
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    } // end while
}

void app_main()
{
    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create mutex");
        return; // Thoát nếu không thể tạo Mutex
    }

    // Khai bao chan GPIO ket noi voi Sensor
    setDHTgpio(GPIO_NUM_4);

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_log_level_set("*", ESP_LOG_INFO);

    esp_rom_gpio_pad_select_gpio(GPIO_NUM_4);

    if (lora_init() == 0)
    {
        ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
        while (1)
        {
            vTaskDelay(1);
        }
    }

    ESP_LOGI(pcTaskGetName(NULL), "Frequency is 433MHz");
    lora_set_frequency(433e6); // 433MHz

    lora_enable_crc();

    int cr = 1;
    int bw = 7;
    int sf = 7;
#if CONFIF_ADVANCED
    cr = CONFIG_CODING_RATE
        bw = CONFIG_BANDWIDTH;
    sf = CONFIG_SF_RATE;
#endif

    lora_set_coding_rate(cr);
    // lora_set_coding_rate(CONFIG_CODING_RATE);
    // cr = lora_get_coding_rate();
    ESP_LOGI(pcTaskGetName(NULL), "coding_rate=%d", cr);

    lora_set_bandwidth(bw);
    // lora_set_bandwidth(CONFIG_BANDWIDTH);
    // int bw = lora_get_bandwidth();
    ESP_LOGI(pcTaskGetName(NULL), "bandwidth=%d", bw);

    lora_set_spreading_factor(sf);
    // lora_set_spreading_factor(CONFIG_SF_RATE);
    // int sf = lora_get_spreading_factor();
    ESP_LOGI(pcTaskGetName(NULL), "spreading_factor=%d", sf);

    xTaskCreate(&DHT_task, "DHT_task", 1024 * 4, NULL, 5, NULL);
    xTaskCreate(&task_tx, "TX", 1024 * 4, NULL, 5, NULL);
}