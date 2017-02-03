/******************************************************************************
 * Copyright 2013-2014 Esprpessif Systems (Wuxi)
 *
 * FileName: user_webserver.c
 *
 * Description: The web server mode confegration.
 *              Check your hardware connection with the host while use this mode.
 * Modification history:
 *     2014/3/12, v1.0 create this file.
*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "esp_wifi.h"
#include "lwip/sys.h"
//#include "lwip/netdb.h"
#include "lwip/api.h"
#include <cJSON.h>
#include <webserver/user_webserver.h>
#include <weta.h>

//#if ESP_PLATFORM
//#include "user_esp_platform.h"
//#endif


//static struct station_config *sta_conf;
//static struct softap_config *ap_conf;

//LOCAL struct secrty_server_info *sec_server;
//LOCAL struct upgrade_server_info *server;
//struct lewei_login_info *login_info;
//static scaninfo *pscaninfo;

//extern uint16_t scannum;

SemaphoreHandle_t weta_mutex;

extern Weta weta;

/**
 * @brief Remember the JSON code text and write to the Weta store if ready.
 * @param codes
 *
 * I don't want to allocate memory to store the numbers converted from strings
 * so instead convert each number to a byte in turn and write them one by one.
 * The only intfoblem is that we need to write the count of the bytes to storage
 * first. So count the numbers as text first, then parse each number.
 */
void
user_weta_program(const char * json)
{
    //printf("user_weta_program(%s)\n\r", json);

    cJSON *root = cJSON_Parse(json);
    WetaCodePtr address = (WetaCodePtr)cJSON_GetObjectItem(root,"address")->valueint;
    cJSON *codesObj = cJSON_GetObjectItem(root,"codes");
        // Weta expects the codes to be as a raw JSON-like string.
        // (don't want dependency on a C JSON implementation in that code)
    char * codes = cJSON_PrintUnformatted(codesObj);

    xSemaphoreTake(weta_mutex, portMAX_DELAY);
    weta_reset(&weta);
    weta_program_json(&weta, address, codes);
    weta_start(&weta);
    xSemaphoreGive(weta_mutex);

    cJSON_Delete(root);
}

bool
user_weta_query(const char * query, char * json, uint16_t length)
{
    //printf("user_weta_query()...\n\r");

    WetaQuery q = QUERY_NONE;
    if (strcmp(query, "all") == 0)
    {
        q = QUERY_ALL;
    }
    //sprintf(json, "{blah:true}");

    xSemaphoreTake(weta_mutex, portMAX_DELAY);
    bool ok = weta_query(&weta, q, json, length);
    xSemaphoreGive(weta_mutex);

    //printf("leaving user_weta_query()\n\r");
    return ok;
}

/******************************************************************************
 * FunctionName : device_get
 * Description  : set up the device information parmer as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 * Returns      : result
*******************************************************************************/
/*
static int
device_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);

    if (strncmp(path, "manufacture", 11) == 0) {
        jsontree_write_string(js_ctx, "Espressif Systems");
    } else if (strncmp(path, "product", 7) == 0)
    {


    }

    return 0;
}

static struct jsontree_callback device_callback =
    JSONTREE_CALLBACK(device_get, NULL);
*/
/******************************************************************************
 * FunctionName : version_get
 * Description  : set up the device version paramer as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 * Returns      : result
*******************************************************************************/
/*
static int
version_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);
    char string[32];

    if (strncmp(path, "hardware", 8) == 0) {

        sprintf(string, "0.1");

    } else if (strncmp(path, "software", 8) == 0) {
        //sprintf(string, "%d.%d.%d", SDK_VERSION_MAJOR, SDK_VERSION_MINOR, SDK_VERSION_REVISION);
        sprintf(string, "%d.%d.%d", 1, 0, 1);
    }

    jsontree_write_string(js_ctx, string);

    return 0;
}

static struct jsontree_callback version_callback =
    JSONTREE_CALLBACK(version_get, NULL);

JSONTREE_OBJECT(device_tree,
                JSONTREE_PAIR("product", &device_callback),
                JSONTREE_PAIR("manufacturer", &device_callback));
JSONTREE_OBJECT(version_tree,
                JSONTREE_PAIR("hardware", &version_callback),
                JSONTREE_PAIR("software", &version_callback));
JSONTREE_OBJECT(info_tree,
                JSONTREE_PAIR("Version", &version_tree),
                JSONTREE_PAIR("Device", &device_tree));

JSONTREE_OBJECT(INFOTree,
                JSONTREE_PAIR("info", &info_tree));
*/
/*
static int
connect_status_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);

    if (strncmp(path, "status", 8) == 0) {
        jsontree_write_int(js_ctx, user_esp_platform_get_connect_status());
    }

    return 0;
}

static struct jsontree_callback connect_status_callback =
    JSONTREE_CALLBACK(connect_status_get, NULL);

JSONTREE_OBJECT(status_sub_tree,
                JSONTREE_PAIR("status", &connect_status_callback));

JSONTREE_OBJECT(connect_status_tree,
                JSONTREE_PAIR("Status", &status_sub_tree));

JSONTREE_OBJECT(con_status_tree,
                JSONTREE_PAIR("info", &connect_status_tree));
*/

/******************************************************************************
 * FunctionName : wifi_station_get
 * Description  : set up the station paramer as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 * Returns      : result
*******************************************************************************/
/*
static int
wifi_station_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);
    struct ip_info ipconfig;
    uint8_t buf[20];
    os_bzero(buf, sizeof(buf));
    wifi_station_get_config(sta_conf);
    wifi_get_ip_info(STATION_IF, &ipconfig);

    if (strncmp(path, "ssid", 4) == 0) {
        jsontree_write_string(js_ctx, sta_conf->ssid);
    } else if (strncmp(path, "password", 8) == 0) {
        jsontree_write_string(js_ctx, sta_conf->password);
    } else if (strncmp(path, "ip", 2) == 0) {
        sprintf(buf, IPSTR, IP2STR(&ipconfig.ip));
        jsontree_write_string(js_ctx, buf);
    } else if (strncmp(path, "mask", 4) == 0) {
        sprintf(buf, IPSTR, IP2STR(&ipconfig.netmask));
        jsontree_write_string(js_ctx, buf);
    } else if (strncmp(path, "gw", 2) == 0) {
        sprintf(buf, IPSTR, IP2STR(&ipconfig.gw));
        jsontree_write_string(js_ctx, buf);
    }

    return 0;
}
*/
/******************************************************************************
 * FunctionName : wifi_station_set
 * Description  : parse the station parmer as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 *                parser -- A pointer to a JSON parser state
 * Returns      : result
*******************************************************************************/
/*
static int
wifi_station_set(struct jsontree_context *js_ctx, struct jsonparse_state *parser)
{
    int type;
    uint8_t station_tree;

    while ((type = jsonparse_next(parser)) != 0) {
        if (type == JSON_TYPE_PAIR_NAME) {
            char buffer[64];
            os_bzero(buffer, 64);

            if (jsonparse_strcmp_value(parser, "Station") == 0) {
                station_tree = 1;
            } else if (jsonparse_strcmp_value(parser, "Softap") == 0) {
                station_tree = 0;
            }

            if (station_tree) {
                if (jsonparse_strcmp_value(parser, "ssid") == 0) {
                    jsonparse_next(parser);
                    jsonparse_next(parser);
                    jsonparse_copy_value(parser, buffer, sizeof(buffer));
                    memcpy(sta_conf->ssid, buffer, strlen(buffer));
                } else if (jsonparse_strcmp_value(parser, "password") == 0) {
                    jsonparse_next(parser);
                    jsonparse_next(parser);
                    jsonparse_copy_value(parser, buffer, sizeof(buffer));
                    memcpy(sta_conf->password, buffer, strlen(buffer));
                }
            }
        }
    }

    return 0;
}

static struct jsontree_callback wifi_station_callback =
    JSONTREE_CALLBACK(wifi_station_get, wifi_station_set);

JSONTREE_OBJECT(get_station_config_tree,
                JSONTREE_PAIR("ssid", &wifi_station_callback),
                JSONTREE_PAIR("password", &wifi_station_callback));
JSONTREE_OBJECT(set_station_config_tree,
                JSONTREE_PAIR("ssid", &wifi_station_callback),
                JSONTREE_PAIR("password", &wifi_station_callback),
                JSONTREE_PAIR("token", &wifi_station_callback));

JSONTREE_OBJECT(ip_tree,
                JSONTREE_PAIR("ip", &wifi_station_callback),
                JSONTREE_PAIR("mask", &wifi_station_callback),
                JSONTREE_PAIR("gw", &wifi_station_callback));
JSONTREE_OBJECT(get_station_tree,
                JSONTREE_PAIR("Connect_Station", &get_station_config_tree),
                JSONTREE_PAIR("Ipinfo_Station", &ip_tree));
JSONTREE_OBJECT(set_station_tree,
                JSONTREE_PAIR("Connect_Station", &set_station_config_tree));

//JSONTREE_OBJECT(get_wifi_station_info_tree,
//                JSONTREE_PAIR("Station", &get_station_tree));
//JSONTREE_OBJECT(set_wifi_station_info_tree,
//                JSONTREE_PAIR("station", &set_station_tree));
*/
/******************************************************************************
 * FunctionName : wifi_softap_get
 * Description  : set up the softap paramer as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 * Returns      : result
*******************************************************************************/
/*
static int
wifi_softap_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);
    struct ip_info ipconfig;
    uint8_tbuf[20];
    os_bzero(buf, sizeof(buf));
    wifi_softap_get_config(ap_conf);
    wifi_get_ip_info(SOFTAP_IF, &ipconfig);

    if (strncmp(path, "ssid", 4) == 0) {
        jsontree_write_string(js_ctx, ap_conf->ssid);
    } else if (strncmp(path, "password", 8) == 0) {
        jsontree_write_string(js_ctx, ap_conf->password);
    } else if (strncmp(path, "channel", 7) == 0) {
        jsontree_write_int(js_ctx, ap_conf->channel);
    } else if (strncmp(path, "authmode", 8) == 0) {
        switch (ap_conf->authmode) {
            case AUTH_OPEN:
                jsontree_write_string(js_ctx, "OPEN");
                break;

            case AUTH_WEP:
                jsontree_write_string(js_ctx, "WEP");
                break;

            case AUTH_WPA_PSK:
                jsontree_write_string(js_ctx, "WPAPSK");
                break;

            case AUTH_WPA2_PSK:
                jsontree_write_string(js_ctx, "WPA2PSK");
                break;

            case AUTH_WPA_WPA2_PSK:
                jsontree_write_string(js_ctx, "WPAPSK/WPA2PSK");
                break;

            default :
                jsontree_write_int(js_ctx, ap_conf->authmode);
                break;
        }
    } else if (strncmp(path, "ip", 2) == 0) {
        sprintf(buf, IPSTR, IP2STR(&ipconfig.ip));
        jsontree_write_string(js_ctx, buf);
    } else if (strncmp(path, "mask", 4) == 0) {
        sprintf(buf, IPSTR, IP2STR(&ipconfig.netmask));
        jsontree_write_string(js_ctx, buf);
    } else if (strncmp(path, "gw", 2) == 0) {
        sprintf(buf, IPSTR, IP2STR(&ipconfig.gw));
        jsontree_write_string(js_ctx, buf);
    }

    return 0;
}
*/
/******************************************************************************
 * FunctionName : wifi_softap_set
 * Description  : parse the softap parmer as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 *                parser -- A pointer to a JSON parser state
 * Returns      : result
*******************************************************************************/
/*
static int
wifi_softap_set(struct jsontree_context *js_ctx, struct jsonparse_state *parser)
{
    int type;
    uint8_t softap_tree;

    while ((type = jsonparse_next(parser)) != 0) {
        if (type == JSON_TYPE_PAIR_NAME) {
            char buffer[64];
            os_bzero(buffer, 64);

            if (jsonparse_strcmp_value(parser, "Station") == 0) {
                softap_tree = 0;
            } else if (jsonparse_strcmp_value(parser, "Softap") == 0) {
                softap_tree = 1;
            }

            if (softap_tree) {
                if (jsonparse_strcmp_value(parser, "authmode") == 0) {
                    jsonparse_next(parser);
                    jsonparse_next(parser);
                    jsonparse_copy_value(parser, buffer, sizeof(buffer));

                    // other mode will be supported later...
                    if (strcmp(buffer, "OPEN") == 0) {
                        ap_conf->authmode = AUTH_OPEN;
                    } else if (strcmp(buffer, "WPAPSK") == 0) {
                        ap_conf->authmode = AUTH_WPA_PSK;
                        printf("%d %s\n", ap_conf->authmode, buffer);
                    } else if (strcmp(buffer, "WPA2PSK") == 0) {
                        ap_conf->authmode = AUTH_WPA2_PSK;
                    } else if (strcmp(buffer, "WPAPSK/WPA2PSK") == 0) {
                        ap_conf->authmode = AUTH_WPA_WPA2_PSK;
                    } else {
                        ap_conf->authmode = AUTH_OPEN;
                        return 0;
                    }
                }

                if (jsonparse_strcmp_value(parser, "channel") == 0) {
                    jsonparse_next(parser);
                    jsonparse_next(parser);
                    ap_conf->channel = jsonparse_get_value_as_int(parser);
                } else if (jsonparse_strcmp_value(parser, "ssid") == 0) {
                    jsonparse_next(parser);
                    jsonparse_next(parser);
                    jsonparse_copy_value(parser, buffer, sizeof(buffer));
                    memcpy(ap_conf->ssid, buffer, strlen(buffer));
                } else if (jsonparse_strcmp_value(parser, "password") == 0) {
                    jsonparse_next(parser);
                    jsonparse_next(parser);
                    jsonparse_copy_value(parser, buffer, sizeof(buffer));
                    memcpy(ap_conf->password, buffer, strlen(buffer));
                }
            }
        }
    }

    return 0;
}

static struct jsontree_callback wifi_softap_callback =
    JSONTREE_CALLBACK(wifi_softap_get, wifi_softap_set);

JSONTREE_OBJECT(softap_config_tree,
                JSONTREE_PAIR("authmode", &wifi_softap_callback),
                JSONTREE_PAIR("channel", &wifi_softap_callback),
                JSONTREE_PAIR("ssid", &wifi_softap_callback),
                JSONTREE_PAIR("password", &wifi_softap_callback));
JSONTREE_OBJECT(softap_ip_tree,
                JSONTREE_PAIR("ip", &wifi_softap_callback),
                JSONTREE_PAIR("mask", &wifi_softap_callback),
                JSONTREE_PAIR("gw", &wifi_softap_callback));
JSONTREE_OBJECT(get_softap_tree,
                JSONTREE_PAIR("Connect_Softap", &softap_config_tree),
                JSONTREE_PAIR("Ipinfo_Softap", &softap_ip_tree));
JSONTREE_OBJECT(set_softap_tree,
                JSONTREE_PAIR("Ipinfo_Softap", &softap_config_tree));

JSONTREE_OBJECT(get_wifi_tree,
                JSONTREE_PAIR("Station", &get_station_tree),
                JSONTREE_PAIR("Softap", &get_softap_tree));
JSONTREE_OBJECT(set_wifi_tree,
                JSONTREE_PAIR("Station", &set_station_tree),
                JSONTREE_PAIR("Softap", &set_softap_tree));

JSONTREE_OBJECT(wifi_response_tree,
                JSONTREE_PAIR("Response", &get_wifi_tree));
JSONTREE_OBJECT(wifi_request_tree,
                JSONTREE_PAIR("Request", &set_wifi_tree));

JSONTREE_OBJECT(wifi_info_tree,
                JSONTREE_PAIR("wifi", &wifi_response_tree));
JSONTREE_OBJECT(wifi_req_tree,
                JSONTREE_PAIR("wifi", &wifi_request_tree));

*/
/******************************************************************************
 * FunctionName : scan_get
 * Description  : set up the scan data as a JSON format
 * Parameters   : js_ctx -- A pointer to a JSON set up
 * Returns      : result
*******************************************************************************/
/*
static int
scan_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);
    //    STAILQ_HEAD(, bss_info) *pbss = scanarg;
    static struct bss_info *bss;

    if (strncmp(path, "TotalPage", 9) == 0) {
        jsontree_write_int(js_ctx, pscaninfo->totalpage);
    } else if (strncmp(path, "PageNum", 7) == 0) {
        jsontree_write_int(js_ctx, pscaninfo->pagenum);
    } else if (strncmp(path, "bssid", 5) == 0) {
        bss = STAILQ_FIRST(pscaninfo->pbss);
        uint8_t buffer[32];
        //if (bss != NULL){
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, MACSTR, MAC2STR(bss->bssid));
        jsontree_write_string(js_ctx, buffer);
        //}
    } else if (strncmp(path, "ssid", 4) == 0) {
        //if (bss != NULL)
        jsontree_write_string(js_ctx, bss->ssid);
    } else if (strncmp(path, "rssi", 4) == 0) {
        //if (bss != NULL)
        jsontree_write_int(js_ctx, -(bss->rssi));
    } else if (strncmp(path, "channel", 7) == 0) {
        //if (bss != NULL)
        jsontree_write_int(js_ctx, bss->channel);
    } else if (strncmp(path, "authmode", 8) == 0) {
        //if (bss != NULL){
        switch (bss->authmode) {
            case AUTH_OPEN:
                jsontree_write_string(js_ctx, "OPEN");
                break;

            case AUTH_WEP:
                jsontree_write_string(js_ctx, "WEP");
                break;

            case AUTH_WPA_PSK:
                jsontree_write_string(js_ctx, "WPAPSK");
                break;

            case AUTH_WPA2_PSK:
                jsontree_write_string(js_ctx, "WPA2PSK");
                break;

            case AUTH_WPA_WPA2_PSK:
                jsontree_write_string(js_ctx, "WPAPSK/WPA2PSK");
                break;

            default :
                jsontree_write_int(js_ctx, bss->authmode);
                break;
        }

        STAILQ_REMOVE_HEAD(pscaninfo->pbss, next);
        free(bss);
        //}
    }

    return 0;
}

static struct jsontree_callback scan_callback =
    JSONTREE_CALLBACK(scan_get, NULL);

JSONTREE_OBJECT(scaninfo_tree,
                JSONTREE_PAIR("bssid", &scan_callback),
                JSONTREE_PAIR("ssid", &scan_callback),
                JSONTREE_PAIR("rssi", &scan_callback),
                JSONTREE_PAIR("channel", &scan_callback),
                JSONTREE_PAIR("authmode", &scan_callback));
JSONTREE_ARRAY(scanrslt_tree,
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree),
               JSONTREE_PAIR_ARRAY(&scaninfo_tree));

JSONTREE_OBJECT(scantree,
                JSONTREE_PAIR("TotalPage", &scan_callback),
                JSONTREE_PAIR("PageNum", &scan_callback),
                JSONTREE_PAIR("ScanResult", &scanrslt_tree));
JSONTREE_OBJECT(scanres_tree,
                JSONTREE_PAIR("Response", &scantree));
JSONTREE_OBJECT(scan_tree,
                JSONTREE_PAIR("scan", &scanres_tree));
*/
/******************************************************************************
 * FunctionName : parse_url
 * Description  : parse the received data from the server
 * Parameters   : precv -- the received data
 *                purl_frame -- the result of parsing the url
 * Returns      : none
*******************************************************************************/
static void
parse_url(char *precv, URL_Frame *purl_frame)
{
    char *str = NULL;
    uint8_t length = 0;
    char *pbuffer = NULL;
    char *pbufer = NULL;

    if (purl_frame == NULL || precv == NULL) {
        return;
    }

    pbuffer = (char *)strstr(precv, "Host:");

    if (pbuffer != NULL) {
        length = pbuffer - precv;
        pbufer = (char *)calloc(length + 1, 1);
        pbuffer = pbufer;
        memcpy(pbuffer, precv, length);
        memset(purl_frame->pSelect, 0, URLSize);
        memset(purl_frame->pCommand, 0, URLSize);
        memset(purl_frame->pFilename, 0, URLSize);

        if (strncmp(pbuffer, "GET ", 4) == 0) {
            purl_frame->Type = GET;
            pbuffer += 4;
        } else if (strncmp(pbuffer, "POST ", 5) == 0) {
            purl_frame->Type = POST;
            pbuffer += 5;
        }
        else if (strncmp(pbuffer, "OPTIONS ", 8) == 0) {
            purl_frame->Type = OPTIONS;
            pbuffer += 8;
        }

        pbuffer ++;
        str = (char *)strstr(pbuffer, "?");

        if (str != NULL) {
            length = str - pbuffer;
            memcpy(purl_frame->pSelect, pbuffer, length);
            str ++;
            pbuffer = (char *)strstr(str, "=");

            if (pbuffer != NULL) {
                length = pbuffer - str;
                memcpy(purl_frame->pCommand, str, length);
                pbuffer ++;
                str = (char *)strstr(pbuffer, "&");

                if (str != NULL) {
                    length = str - pbuffer;
                    memcpy(purl_frame->pFilename, pbuffer, length);
                } else {
                    str = (char *)strstr(pbuffer, " HTTP");

                    if (str != NULL) {
                        length = str - pbuffer;
                        memcpy(purl_frame->pFilename, pbuffer, length);
                    }
                }
            }
        }

        free(pbufer);
    } else {
        return;
    }
}

static char *precvbuffer;
static uint32_t dat_sumlength = 0;
static bool save_data(char *precv, uint16_t length)
{
    //bool flag = false;
    char length_buf[10] = {0};
    char *ptemp = NULL;
    char *pdata = NULL;
    uint16_t headlength = 0;
    static uint32_t totallength = 0;

    ptemp = strstr(precv, "\r\n\r\n");

    if (ptemp != NULL) {
        length -= ptemp - precv;
        length -= 4;
        totallength += length;
        headlength = ptemp - precv + 4;
        pdata = strstr(precv, "Content-Length: ");

        if (pdata != NULL) {
            pdata += 16;
            precvbuffer = strstr(pdata, "\r\n");

            if (precvbuffer != NULL) {
                memcpy(length_buf, pdata, precvbuffer - pdata);
                dat_sumlength = atoi(length_buf);
            }
        } else {
        	if (totallength != 0x00){
        		totallength = 0;
        		dat_sumlength = 0;
               // printf("save_data() failed: No 'Content-Length' header\n");
        		return false;
        	}
        }

        precvbuffer = (char *)calloc(dat_sumlength + headlength + 1, 1);
        memcpy(precvbuffer, precv, strlen(precv));
    } else {
        if (precvbuffer != NULL) {
            totallength += length;
            memcpy(precvbuffer + strlen(precvbuffer), precv, length);
        } else {
            totallength = 0;
            dat_sumlength = 0;
            //printf("save_data() failed: No CRLF pair and receive buffer NULL\n");
            return false;
        }
    }

    if (totallength == dat_sumlength) {
        totallength = 0;
        dat_sumlength = 0;
        return true;
    } else {
        //printf("save_data() failed: totallength = %d, dat_sumlength = %d\n", totallength, dat_sumlength);
        return false;
    }
}

//static os_timer_t *restart_10ms;
//static rst_parm *rstparm;

/******************************************************************************
 * FunctionName : restart_10ms_cb
 * Description  : system restart or wifi reconnected after a certain time.
 * Parameters   : arg -- Additional argument to pass to the function
 * Returns      : none
*******************************************************************************/
/*
static void
restart_10ms_cb(void *arg)
{
    if (rstparm != NULL && rstparm->pespconn != NULL) {
        switch (rstparm->parmtype) {
            case WIFI:
                //if (rstparm->pespconn->state == ESPCONN_CLOSE) {
                    if (sta_conf->ssid[0] != 0x00) {
                        wifi_station_set_config(sta_conf);
                        wifi_station_disconnect();
                        wifi_station_connect();
                        user_esp_platform_check_ip();
                    }

                    if (ap_conf->ssid[0] != 0x00) {
                        wifi_softap_set_config(ap_conf);
                        system_restart();
                    }

                    free(ap_conf);
                    ap_conf = NULL;
                    free(sta_conf);
                    sta_conf = NULL;
                    free(rstparm);
                    rstparm = NULL;
                    free(restart_10ms);
                    restart_10ms = NULL;
                //} else {
                //   os_timer_arm(restart_10ms, 10, 0);
                //}

                break;

            case DEEP_SLEEP:
            case REBOOT:
                if (rstparm->pespconn->state == ESPCONN_CLOSE)
                {
                    wifi_set_opmode(STATION_MODE);

                    if (rstparm->parmtype == DEEP_SLEEP)
                    {

                    }
                } else {
                    os_timer_arm(restart_10ms, 10, 0);
                }

                break;

            default:
                break;
        }
    }
}
*/
/******************************************************************************
 * FunctionName : data_send
 * Description  : processing the data as http format and send to the client or server
 * Parameters   : arg -- argument to set for client or server
 *                responseOK -- true or false
 *                psend -- The send data
 * Returns      :
*******************************************************************************/
static void
data_send(struct netconn *conn, bool responseOK, bool allow, char *psend)
{
    uint16_t length = 0;
    char *pbuf = NULL;
    char httphead[256];
    memset(httphead, 0, 256);

    if (allow)
    {
        sprintf(httphead,
                "HTTP/1.0 200 OK\r\n"
                "Server: lwIP/1.4.0\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type\r\n\r\n"
        );
        length = strlen(httphead);
    }
    else if (responseOK) {
        sprintf(httphead,
                "HTTP/1.0 200 OK\r\n"
        "Content-Length: %d\r\n"
                           "Server: lwIP/1.4.0\r\n"
                           "Access-Control-Allow-Origin: *\r\n",
                   psend ? strlen(psend) : 0);

        if (psend) {
            sprintf(httphead + strlen(httphead),
                       "Content-type: application/json\r\nExpires: Fri, 10 Apr 2008 14:00:00 GMT\r\nPragma: no-cache\r\n\r\n");
            length = strlen(httphead) + strlen(psend);
            pbuf = (char *)calloc(length + 1, 1);
            memcpy(pbuf, httphead, strlen(httphead));
            memcpy(pbuf + strlen(httphead), psend, strlen(psend));
        } else {
            sprintf(httphead + strlen(httphead), "\n");
            length = strlen(httphead);
        }
    } else {
        sprintf(httphead, "HTTP/1.0 400 BadRequest\r\nContent-Length: 0\r\nServer: lwIP/1.4.0\r\n\n");
        length = strlen(httphead);
    }
    //printf("data_send() about to write...\r\n");
    if (psend)
    {
        netconn_write(conn, pbuf, length, NETCONN_NOCOPY);
    }
    else
    {
        //printf("Sending:\r\n%s", httphead);
        netconn_write(conn, httphead, length, NETCONN_NOCOPY);
    }
    //printf("data_send() ...write done\r\n");
    if (pbuf)
    {
        free(pbuf);
        pbuf = NULL;
    }

}

/******************************************************************************
 * FunctionName : json_send
 * Description  : processing the data as json format and send to the client or server
 * Parameters   : arg -- argument to set for client or server
 *                ParmType -- json format type
 * Returns      : none
*******************************************************************************/
/*
static void
json_send(void *arg, ParmType ParmType)
{
    char *pbuf = NULL;
    pbuf = (char *)calloc(jsonSize, 1);
    struct espconn *ptrespconn = arg;

    switch (ParmType)
    {

        case INFOMATION:
            json_ws_send((struct jsontree_value *)&INFOTree, "info", pbuf);
            break;
        case WIFI:
            json_ws_send((struct jsontree_value *)&wifi_info_tree, "wifi", pbuf);
            break;
         case CONNECT_STATUS:
            json_ws_send((struct jsontree_value *)&con_status_tree, "info", pbuf);
            break;
        case SCAN: {
            u8 i = 0;
            u8 scancount = 0;
            struct bss_info *bss = NULL;
            bss = STAILQ_FIRST(pscaninfo->pbss);

            if (bss == NULL) {
                os_free(pscaninfo);
                pscaninfo = NULL;
                os_sprintf(pbuf, "{\n\"successful\": false,\n\"data\": null\n}");
            } else {
                do {
                    if (pscaninfo->page_sn == pscaninfo->pagenum) {
                        pscaninfo->page_sn = 0;
                        os_sprintf(pbuf, "{\n\"successful\": false,\n\"meessage\": \"repeated page\"\n}");
                        break;
                    }

                    scancount = scannum - (pscaninfo->pagenum - 1) * 8;

                    if (scancount >= 8) {
                        pscaninfo->data_cnt += 8;
                        pscaninfo->page_sn = pscaninfo->pagenum;

                        if (pscaninfo->data_cnt > scannum) {
                            pscaninfo->data_cnt -= 8;
                            os_sprintf(pbuf, "{\n\"successful\": false,\n\"meessage\": \"error page\"\n}");
                            break;
                        }

                        json_ws_send((struct jsontree_value *)&scan_tree, "scan", pbuf);
                    } else {
                        pscaninfo->data_cnt += scancount;
                        pscaninfo->page_sn = pscaninfo->pagenum;

                        if (pscaninfo->data_cnt > scannum) {
                            pscaninfo->data_cnt -= scancount;
                            os_sprintf(pbuf, "{\n\"successful\": false,\n\"meessage\": \"error page\"\n}");
                            break;
                        }

                        char *ptrscanbuf = (char *)os_zalloc(jsonSize);
                        char *pscanbuf = ptrscanbuf;
                        os_sprintf(pscanbuf, ",\n\"ScanResult\": [\n");
                        pscanbuf += os_strlen(pscanbuf);

                        for (i = 0; i < scancount; i ++) {
                            JSONTREE_OBJECT(page_tree,
                                            JSONTREE_PAIR("page", &scaninfo_tree));
                            json_ws_send((struct jsontree_value *)&page_tree, "page", pscanbuf);
                            os_sprintf(pscanbuf + os_strlen(pscanbuf), ",\n");
                            pscanbuf += os_strlen(pscanbuf);
                        }

                        os_sprintf(pscanbuf - 2, "]\n");
                        JSONTREE_OBJECT(scantree,
                                        JSONTREE_PAIR("TotalPage", &scan_callback),
                                        JSONTREE_PAIR("PageNum", &scan_callback));
                        JSONTREE_OBJECT(scanres_tree,
                                        JSONTREE_PAIR("Response", &scantree));
                        JSONTREE_OBJECT(scan_tree,
                                        JSONTREE_PAIR("scan", &scanres_tree));
                        json_ws_send((struct jsontree_value *)&scan_tree, "scan", pbuf);
                        os_memcpy(pbuf + os_strlen(pbuf) - 4, ptrscanbuf, os_strlen(ptrscanbuf));
                        os_sprintf(pbuf + os_strlen(pbuf), "}\n}");
                        os_free(ptrscanbuf);
                    }
                } while (0);
            }

            break;

        }

        default :
            break;
    }

    data_send(ptrespconn, true, false, pbuf);
    free(pbuf);
    pbuf = NULL;
}
*/
/******************************************************************************
 * FunctionName : response_send
 * Description  : processing the send result
 * Parameters   : arg -- argument to set for client or server
 *                responseOK --  true or false
 * Returns      : none
*******************************************************************************/
static void
response_send(struct netconn *conn, bool responseOK)
{
    data_send(conn, responseOK, false, NULL);
}

/******************************************************************************
 * FunctionName : json_scan_cb
 * Description  : processing the scan result
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                status -- scan status
 * Returns      : none
*******************************************************************************/
/*
static void
json_scan_cb(void *arg, STATUS status)
{
    pscaninfo->pbss = arg;

    if (scannum % 8 == 0) {
        pscaninfo->totalpage = scannum / 8;
    } else {
        pscaninfo->totalpage = scannum / 8 + 1;
    }

    JSONTREE_OBJECT(totaltree,
                    JSONTREE_PAIR("TotalPage", &scan_callback));
    JSONTREE_OBJECT(totalres_tree,
                    JSONTREE_PAIR("Response", &totaltree));
    JSONTREE_OBJECT(total_tree,
                    JSONTREE_PAIR("total", &totalres_tree));

    char *pbuf = NULL;
    pbuf = (char *)zalloc(jsonSize);
    json_ws_send((struct jsontree_value *)&total_tree, "total", pbuf);
    data_send(pscaninfo->pespconn, true, false, pbuf);
    free(pbuf);
}
*/

/******************************************************************************
 * FunctionName : webserver_recv
 * Description  : Processing the received data from the server
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
*******************************************************************************/
static void
parse_request(struct netconn *conn, char *pRequest, unsigned short length)
{
    /*
    printf("Request(%d)-->", length);
    unsigned short i;
    for (i = 0; i < length; i++)
    {
        printf("%c", pRequest[i]);
    }
    printf("<--\r\n");
    */
    URL_Frame *pURL_Frame = NULL;
    char *pParseBuffer = NULL;
    bool parse_flag;
    parse_flag = save_data(pRequest, length);

    do {
        if (parse_flag == false) {
        	response_send(conn, false);
        	if (dat_sumlength == 0){
        		if (precvbuffer != NULL){
        			free(precvbuffer);
        			precvbuffer = NULL;
        		}
        	}
            break;
        }

//        os_printf(precvbuffer);
        pURL_Frame = (URL_Frame *)calloc(sizeof(URL_Frame), 1);
        parse_url(precvbuffer, pURL_Frame);

        printf("URL: Type = %d, Select = %s, Command = %s, Filename: %s \n",
               pURL_Frame->Type, pURL_Frame->pSelect, pURL_Frame->pCommand, pURL_Frame->pFilename
        );

        switch (pURL_Frame->Type) {
            case GET:
                //("We have a GET request.\n");
                if (strcmp(pURL_Frame->pSelect, "weta") == 0 &&
                    strcmp(pURL_Frame->pCommand, "query") == 0)
                {
                    char json[255]; // TODO: deal with this properly
                    json[0] = 0;
                    //char json[] = "{blah:true}";
                    if (user_weta_query(pURL_Frame->pFilename, json, sizeof(json)))
                        data_send(conn, true, false, json);
                    else
                        response_send(conn, false);
                }
                /*
                if (strcmp(pURL_Frame->pSelect, "client") == 0 &&
                        strcmp(pURL_Frame->pCommand, "command") == 0) {
                    if (strcmp(pURL_Frame->pFilename, "info") == 0) {
                        json_send(ptrespconn, INFOMATION);
                    }

                    if (strcmp(pURL_Frame->pFilename, "status") == 0)
                    {
                        json_send(ptrespconn, CONNECT_STATUS);
                    }
                    else if (strcmp(pURL_Frame->pFilename, "scan") == 0)
                    {
                        char *strstr = NULL;
                        strstr = (char *)strstr(pusrdata, "&");

                        if (strstr == NULL) {
                            if (pscaninfo == NULL) {
                                pscaninfo = (scaninfo *)zalloc(sizeof(scaninfo));
                            }

                            pscaninfo->pespconn = ptrespconn;
                            pscaninfo->pagenum = 0;
                            pscaninfo->page_sn = 0;
                            pscaninfo->data_cnt = 0;
                            wifi_station_scan(NULL, json_scan_cb);
                        } else {
                            strstr ++;

                            if (strncmp(strstr, "page", 4) == 0) {
                                if (pscaninfo != NULL) {
                                    pscaninfo->pagenum = *(strstr + 5);
                                    pscaninfo->pagenum -= 0x30;

                                    if (pscaninfo->pagenum > pscaninfo->totalpage || pscaninfo->pagenum == 0) {
                                        response_send(ptrespconn, false);
                                    } else {
                                        json_send(ptrespconn, SCAN);
                                    }
                                } else {
                                    response_send(ptrespconn, false);
                                }
                            } else {
                                response_send(ptrespconn, false);
                            }
                        }
                    } else {
                        response_send(ptrespconn, false);
                    }
                } else if (strcmp(pURL_Frame->pSelect, "config") == 0 &&
                           strcmp(pURL_Frame->pCommand, "command") == 0)
                {
                    if (strcmp(pURL_Frame->pFilename, "wifi") == 0)
                    {
                        ap_conf = (struct softap_config *)zalloc(sizeof(struct softap_config));
                        sta_conf = (struct station_config *)zalloc(sizeof(struct station_config));
                        json_send(ptrespconn, WIFI);
                        free(sta_conf);
                        free(ap_conf);
                        sta_conf = NULL;
                        ap_conf = NULL;
                    }
                    else if (strcmp(pURL_Frame->pFilename, "reboot") == 0) {
                        json_send(ptrespconn, REBOOT);
                    } else {
                        response_send(ptrespconn, false);
                    }
                } else {
                    response_send(ptrespconn, false);
                }
                */
                break;

            case OPTIONS:
                data_send(conn, true, true, NULL);
                break;

            case POST:
                printf("We have a POST request.\n");
                pParseBuffer = strstr(precvbuffer, "\r\n\r\n");

                if (pParseBuffer == NULL) {
                    break;
                }

                pParseBuffer += 4;

                if (strcmp(pURL_Frame->pSelect, "weta") == 0 &&
                        strcmp(pURL_Frame->pCommand, "cmd") == 0)
                {
                    if (strcmp(pURL_Frame->pFilename, "reboot") == 0)
                    {
                        /*
                        if (pParseBuffer != NULL)
                        {
                            if (restart_10ms != NULL)
                            {
                                os_timer_disarm(restart_10ms);
                            }

                            if (rstparm == NULL) {
                                rstparm = (rst_parm *)calloc(sizeof(rst_parm), 1);
                            }

                            rstparm->pespconn = ptrespconn;
                            rstparm->parmtype = REBOOT;

                            if (restart_10ms == NULL)
                            {
                                restart_10ms = (os_timer_t *)malloc(sizeof(os_timer_t));
                            }

                            os_timer_setfn(restart_10ms, (os_timer_func_t *)restart_10ms_cb, NULL);
                            os_timer_arm(restart_10ms, 10, 0);  // delay 10ms, then do

                            response_send(ptrespconn, true);
                        } else
                        {
                            response_send(ptrespconn, false);
                        }
                         */
                    } else if (strcmp(pURL_Frame->pFilename, "wifi") == 0)
                    {
                        /*
                        if (pParseBuffer != NULL) {
                            struct jsontree_context js;
                            user_esp_platform_set_connect_status(DEVICE_CONNECTING);

                            if (restart_10ms != NULL) {
                                os_timer_disarm(restart_10ms);
                            }

                            if (ap_conf == NULL) {
                                ap_conf = (struct softap_config *)calloc(sizeof(struct softap_config), 1);
                            }

                            if (sta_conf == NULL) {
                                sta_conf = (struct station_config *)calloc(sizeof(struct station_config), 1);
                            }

                            jsontree_setup(&js, (struct jsontree_value *)&wifi_req_tree, json_putchar);
                            json_parse(&js, pParseBuffer);

                            if (rstparm == NULL) {
                                rstparm = (rst_parm *)calloc(sizeof(rst_parm), 1);
                            }

                            rstparm->pespconn = ptrespconn;
                            rstparm->parmtype = WIFI;

                            if (sta_conf->ssid[0] != 0x00 || ap_conf->ssid[0] != 0x00) {
                                ap_conf->ssid_hidden = 0;
                                ap_conf->max_connection = 4;

                                if (restart_10ms == NULL) {
                                    restart_10ms = (os_timer_t *)malloc(sizeof(os_timer_t));
                                }

                                os_timer_disarm(restart_10ms);
                                os_timer_setfn(restart_10ms, (os_timer_func_t *)restart_10ms_cb, NULL);
                                os_timer_arm(restart_10ms, 10, 0);  // delay 10ms, then do
                            } else {
                                free(ap_conf);
                                free(sta_conf);
                                free(rstparm);
                                sta_conf = NULL;
                                ap_conf = NULL;
                                rstparm =NULL;
                            }

                            response_send(ptrespconn, true);
                        } else {
                            response_send(ptrespconn, false);
                        }
                         */
                    }
                    else if (strcmp(pURL_Frame->pFilename, "program") == 0)
                    {
                        printf("Web server received a weta command\n");
                        if (pParseBuffer != NULL) {
                            user_weta_program(pParseBuffer);
                            response_send(conn, true);
                        } else {
                            response_send(conn, false);
                        }
                    }


                    else {
                        response_send(conn, false);
                    }
                } else {
                    response_send(conn, false);
                }

//	            os_free(pParseBuffer);
                break;
        }

        if (precvbuffer != NULL){
        	free(precvbuffer);
        	precvbuffer = NULL;
        }
        free(pURL_Frame);
        pURL_Frame = NULL;

    } while (0);

}

void
init_webserver()
{
    vSemaphoreCreateBinary(weta_mutex);
}

static void
http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    uint16_t buflen;
    err_t err;

    /* Read the data from the port, blocking if nothing yet there.
     We assume the request (the part we care about) is in one netbuf */
    err = netconn_recv(conn, &inbuf);

    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void**)&buf, &buflen);
        parse_request(conn, buf, buflen);
    }
    /* Close the connection (server closes in HTTP) */
    netconn_close(conn);

    /* Delete the buffer (netconn_recv gives us ownership,
     so we have to make sure to deallocate the buffer) */
    netbuf_delete(inbuf);
}

void webserver_task(void *pvParameters)
{
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, SERVER_PORT);
    netconn_listen(conn);
    do {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK) {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
        hw_time_waitms(1);
    } while(err == ERR_OK);
    netconn_close(conn);
    netconn_delete(conn);
}


