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

//WIFI CONNECTION RELATED
static uint8_t _esp8266_smartconfig_wifi_connected;
static os_timer_t _esp8266_smartconfig_wifi_timer;

//OPERATIONAL MODE RELATED
static ESP8266_SMARTCONFIG_MODE _esp8266_smartconfig_mode;
static uint8_t _esp8266_smartconfig_gpio_num;

//CALLBACK FUNCTION VARIABLES
static void (*_esp8266_smartconfig_user_cb_function)(void);
static void (*_esp8266_smartconfig_smartconfig_activated_user_cb_function)(void);
static void (*_esp8266_smartconfig_smartconfig_not_activated_user_cb_function)(void);

void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetDebug(uint8_t debug_on)
{
    //SET DEBUG PRINTF ON(1) OR OFF(0)
    
    _esp8266_smartconfig_debug = debug_on;
}

void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Initialize(ESP8266_SMARTCONFIG_MODE mode, uint8_t gpio)
{
    //INITIALIZE SMARTCONFIG IN THE MODE SPECIFIED
    //GPIO : GPIO PIN READ VALUE IS USED TO DETERMINE IF BOOTING IN THE NORMAL MODE OR SMARTCONFIG
    //WIFI_TIMEOUT : IF CONNECTION TO WIFI FAILS(USING STORED CREDENTIALS), START SMARTCONFIG
    
    _esp8266_smartconfig_mode = mode;
    _esp8266_smartconfig_gpio_num = gpio;

    if(mode == ESP8266_SMARTCONFIG_MODE_GPIO)
    {
        //GPIO MODE

        //SET THE GPIO PIN TO INPUT WILL PULLUP ENABLED
        _esp8266_smartconfig_set_gpio_pin(gpio);

        if(_esp8266_smartconfig_debug)
        {
            os_printf("ESP8266 : SMARTCONFIG : Initialized in GPIO mode on pin %u\n", _esp8266_smartconfig_gpio_num);
        }
    }
    else
    {
        //WIFI TIMEOUT MODE
        
        if(_esp8266_smartconfig_debug)
        {
            os_printf("ESP8266 : SMARTCONFIG : Initialized in WIFI TIMEOUT mode\n");
        }
    }

    _esp8266_smartconfig_wifi_connected = 0;
}

void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_SetCbFunctions(void (*sc_done)(void), void (*sc_activated)(void), void (*sc_not_activated)(void))
{
    //SET THE USER CB FUNCTION THAT WOULD BE CALLED
    //WHEN SMARTCONFIG IS FINISHED SUCCESSFULLY

    _esp8266_smartconfig_user_cb_function = sc_done;
    _esp8266_smartconfig_smartconfig_activated_user_cb_function = sc_activated;
    _esp8266_smartconfig_smartconfig_not_activated_user_cb_function = sc_not_activated;

    if(_esp8266_smartconfig_debug)
    {
        os_printf("ESP8266 : SMARTCONFIG : User cb functions set\n");
    }
}

//CONTROL FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SMARTCONFIG_Start(void)
{
    //START SMARTCONFIG
    //DEPENDING ON THE OPERATIONAL MODE, EITHER USE GPIO TO DETERMINE
    //IF TO START IN SMARTCONFIG MODE OR USE WIFI TIMEOUT
    
    if(_esp8266_smartconfig_debug)
    {
        os_printf("ESP8266 : SMARTCONFIG : Started\n");
    }

    //SET WIFI EVENTS CB FUNCTION
    wifi_set_event_handler_cb(_esp8266_smartconfig_wifi_event_handler_cb);

    if(_esp8266_smartconfig_mode == ESP8266_SMARTCONFIG_MODE_GPIO)
    {
        //GPIO MODE

        if(_esp8266_smartconfig_get_gpio_reading(_esp8266_smartconfig_gpio_num) == 0)
        {
            //GPIO PRESSED. START SMARTCONFIG

            //CALL THE SMARTCONFIG ACTIVATED USER CB IF NOT NULL
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : Smartconfig activated user cb executed\n");
            }
            
            if(_esp8266_smartconfig_smartconfig_activated_user_cb_function != NULL)
            {
                (*_esp8266_smartconfig_smartconfig_activated_user_cb_function)();
            }

            //SET THE SYSTEM IN STATION MODE - SMARTCONFIG REQUIREMENT
            wifi_set_opmode(STATION_MODE);

            //START SMARTCONFIG
            smartconfig_start(_esp8266_smartconfig_smartconfigevents_cb, SC_TYPE_ESPTOUCH);
            
        }
        else
        {
            //GPIO NOT PRESSED. NORMAL BOOT MODE
            
            //CALL THE SMARTCONFIG NOT ACTIVATED USER CB IF NOT NULL
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : Smartconfig not activated user cb executed\n");
            }
            
            if(_esp8266_smartconfig_smartconfig_not_activated_user_cb_function != NULL)
            {
                (*_esp8266_smartconfig_smartconfig_not_activated_user_cb_function)();
            }

            wifi_station_connect();
        }
    }
    else
    {
        //WIFI TIMEOUT MODE
    }
}

//INTERNAL GPIO FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_smartconfig_set_gpio_pin(uint8_t gpio_num)
{
    //SET THE SPECIFIED GPIO PIN MODE TO INPUT + PULL UP
    
    switch(gpio_num)
    {
        case 0:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));
            break;
        case 2:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(2));
            break;
        case 4:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(4));
            break;
        case 5:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));
            break;
        case 12:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(12));
            break;
        case 13:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_MTCK_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));
            break;
        case 14:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));
            break;
        case 15:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
            PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDO_U);
            GPIO_DIS_OUTPUT(GPIO_ID_PIN(15));
            break;
        default:
            os_printf("ESP8266 : SMARTCONFIG : gpio set not supported for %d\n", gpio_num);
    }
    
    if(_esp8266_smartconfig_debug)
    {
        os_printf("ESP8266 : SMARTCONFIG : Set gpio %d to input + pullup\n", gpio_num);
    }
}

uint8_t ICACHE_FLASH_ATTR _esp8266_smartconfig_get_gpio_reading(uint8_t gpio_num)
{
    //RETURN THE READING OF THE SPECIFIED GPIO PIN

    switch(gpio_num)
    {
        case 0:
        case 2:
        case 4:
        case 5:
        case 12:
        case 13:
        case 14:
        case 15:
            return GPIO_INPUT_GET(gpio_num);
            break;
        default:
            os_printf("ESP8266 : SMARTCONFIG : gpio read value not supported for %d\n", gpio_num);
    }
}

//INTERNAL CB FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_smartconfig_wifi_event_handler_cb(System_Event_t* event)
{
    //INTERNAL CB FUNCTION FOR WIFI EVENTS
    
    switch(event->event)
    {
        case EVENT_STAMODE_CONNECTED:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : wifi event CONNECTED\n");
            }
            break;
        case EVENT_STAMODE_DISCONNECTED:
            _esp8266_smartconfig_wifi_connected = 0;
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : wifi event DISCONNECTED\n");
            }
            break;
        case EVENT_STAMODE_AUTHMODE_CHANGE:
           if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : wifi event AUTHMODE_CHANGE\n");
            } 
            break;
        case EVENT_STAMODE_GOT_IP:
            //DEVICE CONNECTED TO WIFI. CALL USER CB FUNCTION
            //TO START THE APPLICATION
            //CALL USER CB IF NOT NULL
            _esp8266_smartconfig_wifi_connected = 1;
            if(_esp8266_smartconfig_user_cb_function != NULL)
            {
                (*_esp8266_smartconfig_user_cb_function)();
            }
            break;
        case EVENT_SOFTAPMODE_STACONNECTED:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : wifi event SOFTAP STA CONNECTED\n");
            } 
            break;
        case EVENT_SOFTAPMODE_STADISCONNECTED:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : wifi event SOFTAO STA DISCONNECTED\n");
            } 
            break;
        default:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : Unknow wifi event %d\n", event->event);
            }
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
            
            wifi_set_event_handler_cb(_esp8266_smartconfig_wifi_event_handler_cb);
            wifi_station_connect();

            //SET AUTO CONNECT = ON
            wifi_station_set_auto_connect(1);
            break;

        case SC_STATUS_LINK_OVER:
            if(_esp8266_smartconfig_debug)
            {
                os_printf("ESP8266 : SMARTCONFIG : LINK OVER\n");
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
            break;
    }
}

