/******************************************************************************
 * Copyright 2013-2014 Esprpessif Systems (Wuxi)
 *
 * FileName: user_webserver.c
 *
 * Description: The web server mode configration.
 *              Check your hardware connection with the host while use this mode.
 * Modification history:
 *     2014/3/12, v1.0 create this file.
*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include <cJSON.h>
#include "user_webserver.h"
#include <weta.h>

xSemaphoreHandle weta_mutex;

extern Weta weta;

void
user_weta_program(const char * json, WetaStorage storage)
{
    //DEBUGMSG("user_weta_program(%s)\n\r", json);

    cJSON *root = cJSON_Parse(json);
    WetaCodePtr address = (WetaCodePtr)cJSON_GetObjectItem(root,"address")->valueint;
    cJSON *codesObj = cJSON_GetObjectItem(root,"codes");
        // Weta expects the codes to be as a raw JSON-like string.
        // (don't want dependency on a C JSON implementation in that code)
    char * codes = cJSON_PrintUnformatted(codesObj);

    xSemaphoreTake(weta_mutex, portMAX_DELAY);
    weta_reset(&weta);
    if(weta_program_json(&weta, storage, address, codes))
    {
        weta_start(&weta);
    }
    else
    {
        weta_debug(&weta, "weta_program_json() failed\n\r");
    }
    xSemaphoreGive(weta_mutex);

    cJSON_Delete(root);
}

bool
user_weta_query(const char * query, char * json, uint16_t length)
{
    //DEBUGMSG("user_weta_query()...\n\r");

    WetaQuery q = QUERY_NONE;
    if (strcmp(query, "all") == 0)
    {
        q = QUERY_ALL;
    }
    //sprintf(json, "{blah:true}");

    xSemaphoreTake(weta_mutex, portMAX_DELAY);
    bool ok = weta_query(&weta, q, json, length);
    xSemaphoreGive(weta_mutex);

    //DEBUGMSG("leaving user_weta_query()\n\r");
    return ok;
}

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
               // DEBUGMSG("save_data() failed: No 'Content-Length' header\n");
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
            //DEBUGMSG("save_data() failed: No CRLF pair and receive buffer NULL\n");
            return false;
        }
    }

    if (totallength == dat_sumlength) {
        totallength = 0;
        dat_sumlength = 0;
        return true;
    } else {
        //DEBUGMSG("save_data() failed: totallength = %d, dat_sumlength = %d\n", totallength, dat_sumlength);
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
    //DEBUGMSG("data_send() about to write...\r\n");
    if (psend)
    {
        netconn_write(conn, pbuf, length, NETCONN_NOCOPY);
    }
    else
    {
        //DEBUGMSG("Sending:\r\n%s", httphead);
        netconn_write(conn, httphead, length, NETCONN_NOCOPY);
    }
    //DEBUGMSG("data_send() ...write done\r\n");
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
    DEBUGMSG("Request(%d)-->", length);
    unsigned short i;
    for (i = 0; i < length; i++)
    {
        DEBUGMSG("%c", pRequest[i]);
    }
    DEBUGMSG("<--\r\n");
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

        DEBUGMSG("URL: Type = %d, Select = %s, Command = %s, Filename: %s \n",
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
                break;

            case OPTIONS:
                data_send(conn, true, true, NULL);
                break;

            case POST:
                DEBUGMSG("We have a POST request.\n");
                pParseBuffer = strstr(precvbuffer, "\r\n\r\n");

                if (pParseBuffer == NULL) {
                    break;
                }

                pParseBuffer += 4;

                if (strcmp(pURL_Frame->pSelect, "weta") == 0 &&
                        strcmp(pURL_Frame->pCommand, "cmd") == 0)
                {
                    if (strcmp(pURL_Frame->pFilename, "program") == 0)
                    {
                        DEBUGMSG("Web server received a weta:cmd:program\n");
                        if (pParseBuffer != NULL) {
                            user_weta_program(pParseBuffer, STORAGE_FLASH);
                            response_send(conn, true);
                        } else {
                            response_send(conn, false);
                        }
                    }
                    else if (strcmp(pURL_Frame->pFilename, "interpret") == 0)
                    {
                        DEBUGMSG("Web server received a weta:cmd:interpret\n");
                        if (pParseBuffer != NULL) {
                            user_weta_program(pParseBuffer, STORAGE_RAM);
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


