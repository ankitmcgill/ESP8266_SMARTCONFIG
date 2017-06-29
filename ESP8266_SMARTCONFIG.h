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

//FUNCTION PROTOTYPES/////////////////////////////////////
//CONFIGURATION FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetDebug(uint8_t debug_on);
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetSmartconfigDoneUserCb(void (*user_cb)(void));

//CONTROL FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Start(void);

//INTERNAL CALLBACK FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_smartconfig_smartconfigevents_cb(sc_status status, void* pdata);
//END FUNCTION PROTOTYPES/////////////////////////////////
#endif


