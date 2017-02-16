/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_webserver.c
 *
 * Description: The web server mode configration.
 *              Check your hardware connection with the host while use this mode.
 * Modification history:
 *     2014/3/12, v1.0 create this file.
*******************************************************************************/
#include <weta_platform.h>
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "osapi.h"
#include "user_interface.h"

#include "espconn.h"
#include "user_json.h"
#include "user_webserver.h"
#include <weta.h>


LOCAL struct station_config *sta_conf;
LOCAL struct softap_config *ap_conf;

//LOCAL struct secrty_server_info *sec_server;
//LOCAL struct upgrade_server_info *server;
//struct lewei_login_info *login_info;
LOCAL scaninfo *pscaninfo;

//extern u16 scannum;

extern Weta weta;
static const char* jsonCodes = 0; // Pointer to received JSON text for program codes
static WetaCodePtr jsonAddress;
static uint8 configSteps = 0;	// Used to determine if a full config has been received

void ICACHE_FLASH_ATTR
user_weta_prepare_for_command(void)
{
    weta_debug(&weta, "user_weta_prepare_for_command()\n\r");
    jsonCodes = 0;
    jsonAddress = 0;
    configSteps = 2;
}

void ICACHE_FLASH_ATTR
user_weta_set_start_address(uint32 address)
{
    weta_debug(&weta, "user_weta_set_start_address()\n\r");
    jsonAddress = address;
    if (--configSteps == 0)
    {
        configSteps = 2;
        weta_reset(&weta);
        weta_program_json(&weta, STORAGE_RAM, jsonAddress, jsonCodes);
        weta_start(&weta);
    }
}

/**
 * @brief Remember the JSON code text and write to the Weta store if ready.
 * @param codes
 *
 * I don't want to allocate memory to store the numbers converted from strings
 * so instead convert each number to a byte in turn and write them one by one.
 * The only problem is that we need to write the count of the bytes to storage
 * first. So count the numbers as text first, then parse each number.
 */
void ICACHE_FLASH_ATTR
user_weta_program_codes(const char * codes)
{
    weta_debug(&weta, "user_weta_program_codes()\n\r");
    jsonCodes = codes;

    if (--configSteps == 0)
    {
        configSteps = 2;
        weta_reset(&weta);
        if(weta_program_json(&weta, STORAGE_RAM, jsonAddress, jsonCodes))
        {
            weta_start(&weta);
        }
        else
        {
            weta_debug(&weta, "weta_program_json() failed\n\r");
        }
    }
}

bool ICACHE_FLASH_ATTR
user_weta_query(const char * query, char * json, uint16_t length)
{
    //DEBUGMSG("user_weta_query()...\n\r");

    WetaQuery q = QUERY_NONE;
    if (strcmp(query, "all") == 0)
    {
        q = QUERY_ALL;
    }
    //weta_printf(json, "{blah:true}");

    bool ok = weta_query(&weta, q, json, length);

    //DEBUGMSG("leaving user_weta_query()\n\r");
    return ok;
}

const char * ICACHE_FLASH_ATTR
weta_jsonparse_get_value(struct jsonparse_state *state)
{
	return &state->json[state->vstart];
}



LOCAL int ICACHE_FLASH_ATTR
weta_codes_get(struct jsontree_context *js_ctx)
{
	/*
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);

    if (os_strncmp(path, "red", 3) == 0) {
        jsontree_write_int(js_ctx, user_light_get_duty(LIGHT_RED));
    } else if (os_strncmp(path, "green", 5) == 0) {
        jsontree_write_int(js_ctx, user_light_get_duty(LIGHT_GREEN));
    } else if (os_strncmp(path, "blue", 4) == 0) {
        jsontree_write_int(js_ctx, user_light_get_duty(LIGHT_BLUE));
    } else if (os_strncmp(path, "freq", 4) == 0) {
        jsontree_write_int(js_ctx, user_light_get_freq());
    }
	*/
    return 0;
}

LOCAL int ICACHE_FLASH_ATTR
weta_codes_set(struct jsontree_context *js_ctx, struct jsonparse_state *parser)
{
    int type;

    while ((type = jsonparse_next(parser)) != 0) {
        if (type == JSON_TYPE_PAIR_NAME) {
            if (jsonparse_strcmp_value(parser, "address") == 0) {
                uint32 address;
                jsonparse_next(parser);
                jsonparse_next(parser);
				address = (uint32)jsonparse_get_value_as_int(parser);
				//weta_printf("Motors selected: %d\n", status);
                user_weta_set_start_address(address);
            } else if (jsonparse_strcmp_value(parser, "codes") == 0) {
                uint8 status;
                jsonparse_next(parser);
                jsonparse_next(parser);
				
				//weta_printf("Motors selected: %s\n", cmd);
				const char* codes = weta_jsonparse_get_value(parser);
                user_weta_program_codes(codes);
            } 
        }
    }

    return 0;
}
LOCAL struct jsontree_callback weta_codes_callback =
    JSONTREE_CALLBACK(weta_codes_get, weta_codes_set);

JSONTREE_OBJECT(weta_cmd_tree,
				JSONTREE_PAIR("address", &weta_codes_callback),
                JSONTREE_PAIR("codes", &weta_codes_callback));

JSONTREE_OBJECT(weta_tree,
                JSONTREE_PAIR("weta", &weta_cmd_tree));


/******************************************************************************
 * FunctionName : parse_url
 * Description  : parse the received data from the server
 * Parameters   : precv -- the received data
 *                purl_frame -- the result of parsing the url
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
parse_url(char *precv, URL_Frame *purl_frame)
{
    char *str = NULL;
    uint8 length = 0;
    char *pbuffer = NULL;
    char *pbufer = NULL;

    if (purl_frame == NULL || precv == NULL) {
        return;
    }

    pbuffer = (char *)os_strstr(precv, "Host:");

    if (pbuffer != NULL) {
        length = pbuffer - precv;
        pbufer = (char *)os_zalloc(length + 1);
        pbuffer = pbufer;
        os_memcpy(pbuffer, precv, length);
        os_memset(purl_frame->pSelect, 0, URLSize);
        os_memset(purl_frame->pCommand, 0, URLSize);
        os_memset(purl_frame->pFilename, 0, URLSize);

        if (strncmp(pbuffer, "OPTIONS ", 8) == 0) {
            purl_frame->Type = OPTIONS;
            pbuffer += 8;
        } else if (os_strncmp(pbuffer, "GET ", 4) == 0) {
            purl_frame->Type = GET;
            pbuffer += 4;
        } else if (os_strncmp(pbuffer, "POST ", 5) == 0) {
            purl_frame->Type = POST;
            pbuffer += 5;
        }

        pbuffer ++;
        str = (char *)os_strstr(pbuffer, "?");

        if (str != NULL) {
            length = str - pbuffer;
            os_memcpy(purl_frame->pSelect, pbuffer, length);
            str ++;
            pbuffer = (char *)os_strstr(str, "=");

            if (pbuffer != NULL) {
                length = pbuffer - str;
                os_memcpy(purl_frame->pCommand, str, length);
                pbuffer ++;
                str = (char *)os_strstr(pbuffer, "&");

                if (str != NULL) {
                    length = str - pbuffer;
                    os_memcpy(purl_frame->pFilename, pbuffer, length);
                } else {
                    str = (char *)os_strstr(pbuffer, " HTTP");

                    if (str != NULL) {
                        length = str - pbuffer;
                        os_memcpy(purl_frame->pFilename, pbuffer, length);
                    }
                }
            }
        }

        os_free(pbufer);
    } else {
        return;
    }
}

LOCAL char *precvbuffer;
static uint32 dat_sumlength = 0;
LOCAL bool save_data(char *precv, uint16 length)
{
    bool flag = false;
    char length_buf[10] = {0};
    char *ptemp = NULL;
    char *pdata = NULL;
    uint16 headlength = 0;
    static uint32 totallength = 0;

    ptemp = (char *)os_strstr(precv, "\r\n\r\n");

    if (ptemp != NULL) {
        length -= ptemp - precv;
        length -= 4;
        totallength += length;
        headlength = ptemp - precv + 4;
        pdata = (char *)os_strstr(precv, "Content-Length: ");

        if (pdata != NULL) {
            pdata += 16;
            precvbuffer = (char *)os_strstr(pdata, "\r\n");

            if (precvbuffer != NULL) {
                os_memcpy(length_buf, pdata, precvbuffer - pdata);
                dat_sumlength = atoi(length_buf);
            }
        } else {
        	if (totallength != 0x00){
        		totallength = 0;
        		dat_sumlength = 0;
        		return false;
        	}
        }

        precvbuffer = (char *)os_zalloc(dat_sumlength + headlength + 1);
        os_memcpy(precvbuffer, precv, os_strlen(precv));
    } else {
        if (precvbuffer != NULL) {
            totallength += length;
            os_memcpy(precvbuffer + os_strlen(precvbuffer), precv, length);
        } else {
            totallength = 0;
            dat_sumlength = 0;
            return false;
        }
    }

    if (totallength == dat_sumlength) {
        totallength = 0;
        dat_sumlength = 0;
        return true;
    } else {
        return false;
    }
}

/******************************************************************************
 * FunctionName : data_send
 * Description  : processing the data as http format and send to the client or server
 * Parameters   : arg -- argument to set for client or server
 *                responseOK -- true or false
 *                psend -- The send data
 * Returns      :
*******************************************************************************/
static void ICACHE_FLASH_ATTR
data_send(void *arg, bool responseOK, bool allow, char *psend)
{
    uint16_t length = 0;
    char *pbuf = NULL;
    char httphead[256];
    struct espconn *ptrespconn = arg;
    os_memset(httphead, 0, 256);

    if (allow)
    {
        weta_sprintf(httphead,
                "HTTP/1.0 200 OK\r\n"
                    "Server: lwIP/1.4.0\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                    "Access-Control-Allow-Headers: Content-Type\r\n\r\n"
        );
        length = strlen(httphead);
    }
    else if (responseOK) {
        weta_sprintf(httphead,
                "HTTP/1.0 200 OK\r\n"
                    "Content-Length: %d\r\n"
                    "Server: lwIP/1.4.0\r\n"
                    "Access-Control-Allow-Origin: *\r\n",
                psend ? strlen(psend) : 0);

        if (psend) {
            weta_sprintf(httphead + strlen(httphead),
                    "Content-type: application/json\r\nExpires: Fri, 10 Apr 2008 14:00:00 GMT\r\nPragma: no-cache\r\n\r\n");
            length = strlen(httphead) + strlen(psend);
            pbuf = (char *)os_zalloc(length + 1);
            memcpy(pbuf, httphead, strlen(httphead));
            memcpy(pbuf + strlen(httphead), psend, strlen(psend));
        } else {
            weta_sprintf(httphead + strlen(httphead), "\n");
            length = strlen(httphead);
        }
    } else {
        weta_sprintf(httphead, "HTTP/1.0 400 BadRequest\r\nContent-Length: 0\r\nServer: lwIP/1.4.0\r\n\n");
        length = strlen(httphead);
    }
    //DEBUGMSG("data_send() about to write...\r\n");
    if (psend) {
#ifdef SERVER_SSL_ENABLE
        espconn_secure_sent(ptrespconn, pbuf, length);
#else
        espconn_sent(ptrespconn, pbuf, length);
#endif
    } else {
#ifdef SERVER_SSL_ENABLE
        espconn_secure_sent(ptrespconn, httphead, length);
#else
        espconn_sent(ptrespconn, httphead, length);
#endif
    }
    //DEBUGMSG("data_send() ...write done\r\n");
    if (pbuf)
    {
        os_free(pbuf);
        pbuf = NULL;
    }
}

/******************************************************************************
 * FunctionName : response_send
 * Description  : processing the send result
 * Parameters   : arg -- argument to set for client or server
 *                responseOK --  true or false
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
response_send(void *arg, bool responseOK)
{
    struct espconn *ptrespconn = arg;

    data_send(ptrespconn, responseOK, false, NULL);
}

/******************************************************************************
 * FunctionName : webserver_recv
 * Description  : Processing the received data from the server
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
webserver_recv(void *arg, char *pusrdata, unsigned short length)
{
    URL_Frame *pURL_Frame = NULL;
    char *pParseBuffer = NULL;
    bool parse_flag = false;
    struct espconn *ptrespconn = arg;
    parse_flag = save_data(pusrdata, length);

    do {
        if (parse_flag == false) {
        	response_send(ptrespconn, false);
        	if (dat_sumlength == 0){
        		if (precvbuffer != NULL){
        			os_free(precvbuffer);
        			precvbuffer = NULL;
        		}
        	}
            break;
        }

//        weta_printf(precvbuffer);
        pURL_Frame = (URL_Frame *)os_zalloc(sizeof(URL_Frame));
        parse_url(precvbuffer, pURL_Frame);

        weta_printf("URL: Type = %d, Select = %s, Command = %s, Filename: %s \n",
                 pURL_Frame->Type, pURL_Frame->pSelect, pURL_Frame->pCommand, pURL_Frame->pFilename
        );

        switch (pURL_Frame->Type) {
            case GET:
                weta_printf("We have a GET request.\n");
                if (strcmp(pURL_Frame->pSelect, "weta") == 0 &&
                    strcmp(pURL_Frame->pCommand, "query") == 0)
                {
                    char json[255]; // TODO: deal with this properly
                    json[0] = 0;
                    //char json[] = "{blah:true}";
                    if (user_weta_query(pURL_Frame->pFilename, json, sizeof(json)))
                        data_send(ptrespconn, true, false, json);
                    else
                        response_send(ptrespconn, false);
                }
                break;

            case OPTIONS:
                data_send(ptrespconn, true, true, NULL);
                break;

            case POST:
                weta_printf("We have a POST request.\n");
                pParseBuffer = (char *)os_strstr(precvbuffer, "\r\n\r\n");

                if (pParseBuffer == NULL) {
                    break;
                }

                pParseBuffer += 4;

                if (os_strcmp(pURL_Frame->pSelect, "weta") == 0 &&
                        os_strcmp(pURL_Frame->pCommand, "cmd") == 0)
                {
                    if (os_strcmp(pURL_Frame->pFilename, "program") == 0)
                    {
                        if (pParseBuffer != NULL) {
							user_weta_prepare_for_command();
                            struct jsontree_context js;

                            jsontree_setup(&js, (struct jsontree_value *)&weta_tree, json_putchar);
                            json_parse(&js, pParseBuffer);
                            response_send(ptrespconn, true);
                        } else {
                            response_send(ptrespconn, false);
                        }
                    }
                    else
                    {
                        response_send(ptrespconn, false);
                    }
                } else
                {
                    response_send(ptrespconn, false);
                }

//	            os_free(pParseBuffer);
                break;
        }

        if (precvbuffer != NULL){
        	os_free(precvbuffer);
        	precvbuffer = NULL;
        }
        os_free(pURL_Frame);
        pURL_Frame = NULL;

    } while (false);
    espconn_disconnect(ptrespconn);
}

/******************************************************************************
 * FunctionName : webserver_recon
 * Description  : the connection has been err, reconnection
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
LOCAL ICACHE_FLASH_ATTR
void webserver_recon(void *arg, sint8 err)
{
    struct espconn *pesp_conn = arg;

    weta_printf("webserver's %d.%d.%d.%d:%d err %d reconnect\n", pesp_conn->proto.tcp->remote_ip[0],
    		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
    		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port, err);
}

/******************************************************************************
 * FunctionName : webserver_recon
 * Description  : the connection has been err, reconnection
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
LOCAL ICACHE_FLASH_ATTR
void webserver_discon(void *arg)
{
    struct espconn *pesp_conn = arg;

    weta_printf("webserver's %d.%d.%d.%d:%d disconnect\n", pesp_conn->proto.tcp->remote_ip[0],
        		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
        		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port);
}

/******************************************************************************
 * FunctionName : user_accept_listen
 * Description  : server listened a connection successfully
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
webserver_listen(void *arg)
{
    struct espconn *pesp_conn = arg;

    espconn_regist_recvcb(pesp_conn, webserver_recv);
    espconn_regist_reconcb(pesp_conn, webserver_recon);
    espconn_regist_disconcb(pesp_conn, webserver_discon);
}

/******************************************************************************
 * FunctionName : user_webserver_init
 * Description  : parameter initialize as a server
 * Parameters   : port -- server port
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_webserver_init(uint32 port)
{
    LOCAL struct espconn esp_conn;
    LOCAL esp_tcp esptcp;

    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;
    esp_conn.proto.tcp = &esptcp;
    esp_conn.proto.tcp->local_port = port;
    espconn_regist_connectcb(&esp_conn, webserver_listen);

#ifdef SERVER_SSL_ENABLE
    espconn_secure_accept(&esp_conn);
#else
    espconn_accept(&esp_conn);
#endif
}
