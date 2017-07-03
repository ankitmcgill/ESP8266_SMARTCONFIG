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
#include "user_interface.h"
#include "mem.h"
#include "smartconfig.h"

#define ESP8266_SMARTCONFIG_WIFI_RETRY_COUNT    3

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
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetCbFunctions(void (*sc_done)(void), void (*sc_activated)(void), void (*sc_not_activated)(void));

//CONTROL FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Start(void);

//INTERNAL GPIO FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_smartconfig_set_gpio_pin(uint8_t gpio_num);
uint8_t ICACHE_FLASH_ATTR _esp8266_smartconfig_get_gpio_reading(uint8_t gpio_num);

//INTERNAL CALLBACK FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_smartconfig_wifi_event_handler_cb(System_Event_t* event);
void ICACHE_FLASH_ATTR _esp8266_smartconfig_smartconfigevents_cb(sc_status status, void* pdata);
//END FUNCTION PROTOTYPES/////////////////////////////////
#endif


