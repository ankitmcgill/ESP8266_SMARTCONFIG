/*************************************************
 *
 * ESP8266 SMARTCONFIG LIBRARY
 * (ESPTOUCH TYPE SUPPORTED ONLY)
 *
 * NOTE: TO ENABLE SMARTCONFIG IN A PROJECT, NEED
 *       TO INCLUDE 'smartconfig' UNDER 'libs'
 *       IN THE PROJECT Makefile
 *
 * JUNE 29 2017
 * 
 * ANKIT BHATNAGAR
 * ANKIT.BHATNAGARINDIA@GMAIL.COM
 *************************************************/

#include "ESP8266_SMARTCONFIG.h"

//LOCAL LIBRARY VARIABLES/////////////////////////////////////
//DEBUG RELATRED
static uint8_t _esp8266_smartconfig_debug;

//CALLBACK FUNCTION VARIABLES
static void (*_esp8266_smartconfig_user_cb_function)(void);

void ICACHE_FLASH_ATTR ESP8266_TCP_GET_SetDebug(uint8_t debug_on)
{
    //SET DEBUG PRINTF ON(1) OR OFF(0)
    
    _esp8266_smartconfig_debug = debug_on;
}

void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetSmartconfigDoneUserCb(void (*user_cb)(void))
{
    //SET THE USER CB FUNCTION THAT WOULD BE CALLED
    //WHEN SMARTCONFIG IS FINISHED SUCCESSFULLY

    _esp8266_smartconfig_user_cb_function = user_cb;

    if(_esp8266_smartconfig_debug)
    {
        os_printf("ESP8266 : SMARTCONFIG : User cb function set\n");
    }
}

//CONTROL FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Start(void)
{
    //START SMARTCONFIG
    
    //SET THE SYSTEM IN STATION MODE - SMARTCONFIG REQUIREMENT
    wifi_set_opmode(STATION_MODE);

    //START SMARTCONFIG
    smartconfig_start(_esp8266_smartconfig_smartconfigevents_cb, SC_TYPE_ESPTOUCH);

    if(_esp8266_smartconfig_debug)
    {
        os_printf("ESP8266 : SMARTCONFIG : Started\n");
    }
}


void ICACHE_FLASH_ATTR _esp8266_smartconfig_smartconfigevents_cb(sc_status status, void* pdata)
{
    //INTERNAL CB FUNCTION FOR SMARTCONFIG EVENTS
    
    switch(status)
    {
        case SC_STATUS_WAIT:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : WAIT\n");
            }
            break;

        case SC_STATUS_FIND_CHANNEL:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : FIND CHANNEL\n");
            }
            break;

        case SC_STATUS_GETTING_SSID_PSWD:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : GETTING SSID/PWD\n");
            }
            sc_type* type = pdata;
            break;

        case SC_STATUS_LINK:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : LINK\n");
            }

            struct station_config* sta_conf = pdata;
            wifi_station_set_config(sta_conf);
            wifi_station_disconnect();
            wifi_station_connect();

            //SET AUTO CONNECT = ON
            wifi_station_set_auto_connect(1);
            break;

        case SC_STATUS_LINK_OVER:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : LINK OVER"\n);
            }
            if(pdata != NULL)
            {
                //ESP_TOUCH TYPE
                uint8_t phone_ip[4] = {0,0,0,0};
                os_memcpy(phone_ip, (uint8_t*)pdata, 4);
                if(_esp8266_smartconfig_debug)
                {
                    os_printf("ESP8266 : SMARTCONFIG : Phone IP = %d.%d.%d.%d\n", phone_ip[0],
                                                                                    phone_ip[1],
                                                                                    phone_ip[2],
                                                                                    phone_ip[3]);
                }
            }
            else
            {
                //ESP_AIRKISS TYPE
                //NOT SUPPORTED - DONT DO ANYTHING
            }
            
            smartconfig_stop();
            //SMARTCONFIG DONE
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : DONE\n");
            }

            //CALL USER CB IF NOT NULL
            if(_esp8266_smartconfig_user_cb_function != NULL)
            {
                (*_esp8266_smartconfig_user_cb_function)();
            }
            break;
    }
}
