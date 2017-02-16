/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/1/1, v1.0 create this file.
*******************************************************************************/
#include <weta_platform.h>
#include <user_interface.h>
#include <osapi.h>
#include <user_webserver.h>

#include <weta.h>

#define user_procTaskPrio 0
#define user_procTaskQueueLen 1
os_event_t user_procTaskQueue[user_procTaskQueueLen];

Weta weta;
extern Hardware hardware;

/******************************************************************************
* FunctionName : user_rf_cal_sector_set
* Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
*                We add this function to force users to set rf cal sector, since
*                we don't know which sector is free in user's application.
*                sector map for last several sectors : ABCCC
*                A : rf cal
    *                B : rf init data
    *                C : sdk parameters
* Parameters   : none
* Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
    case FLASH_SIZE_4M_MAP_256_256:
        rf_cal_sec = 128 - 5;
        break;

    case FLASH_SIZE_8M_MAP_512_512:
        rf_cal_sec = 256 - 5;
        break;

    case FLASH_SIZE_16M_MAP_512_512:
    case FLASH_SIZE_16M_MAP_1024_1024:
        rf_cal_sec = 512 - 5;
        break;

    case FLASH_SIZE_32M_MAP_512_512:
    case FLASH_SIZE_32M_MAP_1024_1024:
        rf_cal_sec = 1024 - 5;
        break;

    default:
        rf_cal_sec = 0;
        break;
    }

    return rf_cal_sec;
}

void WETAFUNCATTR
init_wifi(void)
{
#if defined(WETA_WIFI_MODE_STATION)
    struct ip_info ip_config;
    struct station_config sta_config;
    bzero(&sta_config, sizeof(struct station_config));

    weta_sprintf(sta_config.ssid, "B-LINK_845R");
    weta_sprintf(sta_config.password, "000");
    wifi_station_set_config(&sta_config);
    weta_sprintf("%s\n", __func__);
    wifi_get_ip_info(STATION_IF, &ip_config);
    while(ip_config.ip.addr == 0){
        vTaskDelay(1000 / portTICK_RATE_MS);
        wifi_get_ip_info(STATION_IF, &ip_config);
    }
#elif defined(WETA_WIFI_MODE_AP)
    struct softap_config ap_config;
    bzero(&ap_config, sizeof(struct softap_config));
    weta_sprintf(ap_config.ssid, "ESP8266Weta");
    weta_sprintf(ap_config.password, "Blahdyblah");
    ap_config.authmode = AUTH_WPA2_PSK;
    ap_config.max_connection = 2;
    ap_config.beacon_interval = 100;
    wifi_softap_set_config(&ap_config);
    wifi_softap_dhcps_start();
#endif
}

static void WETAFUNCATTR
weta_task(os_event_t *events)
{
    weta_loop_body(&weta);

    os_delay_us(100);
    system_os_post(user_procTaskPrio, 0, 0 );
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void WETAFUNCATTR
user_init(void)
{

    //system_set_os_print(0);
    uart_init(BIT_RATE_57600, BIT_RATE_57600); // For now
    hw_init();
    weta_stack_init();
    weta_printf("weta_init()\r\n");
    weta_init(&weta, &hardware, 0);
    weta_printf("init_wifi()\r\n");
    init_wifi();
    weta_printf("user_webserver_init()\r\n");
    user_webserver_init(80);

    system_os_task(weta_task, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );

}



