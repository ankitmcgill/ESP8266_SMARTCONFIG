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


#ifndef _ESP8266_SMARTCONFIG_H_
#define _ESP8266_SMARTCONFIG_H_

#include "osapi.h"
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "smartconfig.h"

#define GPIO_PIN_FUNC_SELECT(x)   PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO##x_U, FUNC_GPIO##x)
#define GPIO_PIN_PULLUP_EN(x)     PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO##x_U)
#define GPIO_PIN_DIS_OUTPUT(x)    GPIO_DIS_OUTPUT(GPIO_ID_PIN(x))

#define ESP8266_SMARTCONFIG_WIFI_RETRY_COUNT 3

//CUSTOM VARIABLE STRUCTURES/////////////////////////////
typedef enum
{
  ESP8266_SMARTCONFIG_MODE_GPIO,
  ESP8266_SMARTCONFIG_MODE_WIFITIMEOUT
} ESP8266_SMARTCONFIG_MODE;

//FUNCTION PROTOTYPES/////////////////////////////////////
//CONFIGURATION FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetDebug(uint8_t debug_on);
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Initialize(ESP8266_SMARTCONFIG_MODE mode, uint8_t gpio);
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetSmartconfigDoneUserCb(void (*user_cb)(void));

//CONTROL FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Start(void);

//INTERNAL CALLBACK FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_smartconfig_wifi_event_handler_cb(System_Event_t* event);
void ICACHE_FLASH_ATTR _esp8266_smartconfig_smartconfigevents_cb(sc_status status, void* pdata);
//END FUNCTION PROTOTYPES/////////////////////////////////
#endif


