
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
//#include "esp_system.h"
//#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
//#include "lwip/sys.h"
//#include "lwip/netdb.h"
//#include "lwip/api.h"
//#include "tcpip_adapter.h"
#include <driver/gpio.h> // For debugging only

#include <user_webserver.h>
#include <stdio.h>          // For printf()

#include <weta_platform.h>
#include <weta.h>
#include <arch/hw_defs.h>

#include <driver/ledc.h>    // For debugging pwm


Weta weta;
extern Hardware hardware;


static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void
init_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
#ifdef CONFIG_WETA_WIFI_MODE_AP
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
    wifi_config_t apConfig = {
            .ap = {
                    .ssid = CONFIG_WIFI_SSID,
                    .password = CONFIG_WIFI_PASSWORD,
                    .ssid_len = 0,
                    .channel = 0,
                    .authmode = WIFI_AUTH_WPA2_PSK,
                    .ssid_hidden = 0,
                    .max_connection = 2, // how many clients to allow?
                    .beacon_interval = 100 // default value
            }
    };

    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &apConfig) );
    ESP_ERROR_CHECK( esp_wifi_start() );
#else
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );

    wifi_config_t wifi_config =
        {
        .sta =
            {
                .ssid = CONFIG_WIFI_SSID,
                .password = CONFIG_WIFI_PASSWORD,
                .bssid_set = false
            },
        };

    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
#endif

}

void weta_task(void *pvParameter)
{
    hw_init();
    weta_stack_init();
    weta_init(&weta, &hardware, 0);

    while(1)
    {
            // This mutex is required so that the Weta VM can be reprogrammed
            // at any time through the web server (which also requests the
            // mutex).
        xSemaphoreTake(weta_mutex, portMAX_DELAY);
        weta_loop_body(&weta);
        xSemaphoreGive(weta_mutex);
        hw_time_waitus(10);

        //printf(".");
        //fflush(stdout);
        //hw_time_waitms(100);
        //vTaskDelay(1 / portTICK_RATE_MS);

    }
}

int app_main(void)
{
    nvs_flash_init();

    init_wifi();
    init_webserver();

    xTaskCreate(&webserver_task, "web_server", 2048, NULL, 5, NULL);
    xTaskCreate(weta_task, "weta_task", 2048, NULL, 5, NULL);

    /*
    hw_init();
    weta_stack_init();
    weta_init(&weta, &hardware, 0);

    hw_motor_select(&hardware.motors, 3);
    hw_motor_power(&hardware.motors, 255);
    hw_motor_on(&hardware.motors, true);

    while(1)
    {
        hw_time_waitus(10);
    }
     */
    return 0;
}


