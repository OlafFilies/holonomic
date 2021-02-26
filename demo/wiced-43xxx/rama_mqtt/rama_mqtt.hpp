/**
 ============================================================================
 \name        : Rama Killough a holonomic driving platform
 \author      : Dr. Olaf Filies, Marcus Gordon Filies
 \version     : 0.0.1
 \copyright   : Infineon Technologies AG
 \brief       : Controlling 3 wheel holonomic platform with
 \brief       : Controlling 3 wheel holonomic platform with
                one Infineon TLE94112EL microcontroller shield
                one Infineon XMC1100 or XMC4x00 microcontroller
 used         : Infineon Arduino port for using XMC arm based microcontroller
                Infineon tle94112 base library
                1x Infineon XMC1100 microcontroller
                1x Infineon TLE94112 DC motor controller shield
 ============================================================================
 
 ============================================================================
 Rama holonomic driving platform is on and waiting for input
 ============================================================================
 We need three values
     the angular alpha for the direction between 0deg-360deg
     the speed in alpha direction between 0%-100%
     the speed the rotation -100%-100% speed
         with negative values as anti clockwise
         and positive numbers for clockwise
     either alpha and alpha speed are set or rotation speed are set but
     yet not both
 ============================================================================
 */

#ifndef RAMA_MQTT_HPP_
#define RAMA_MQTT_HPP_

#include "../../config/holonomic-conf.hpp"

#if (HOLONOMIC_FRAMEWORK == HOLONOMIC_FRMWK_WICED)

#include <stdlib.h>
#include <wiced.h>
#include <math.h>
#include <TLE94112-platf-wiced.hpp>
#include <TLE94112-pal-wiced.hpp>
#include <TLE94112.hpp>
#include <TLE94112Motor.hpp>
#include <Holonomic3.hpp>
#include "mqtt_api.h"
#include "resources.h"
#include "mqtt_config.h"


#define RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR( command, ok_message, error_message )   \
        {                                                                                   \
            ret = (command);                                                                \
            mqtt_print_status( ret, (const char *)ok_message, (const char *)error_message );\
            if ( ret != WICED_SUCCESS ) break;                                              \
        }

/******************************************************
 *                   Enumerations
 ******************************************************/
// Main application state machine
typedef enum {
    APP_STATE_AFTER_RESET = 0,
    APP_STATE_CONNECT_TO_WIFI,
    APP_STATE_MQTT_FIND_BROKER,
    APP_STATE_MQTT_CONNECT,
    APP_STATE_MQTT_CLOSECONNECT,
    APP_STATE_MQTT_SUBSCRIBE,
    APP_STATE_MQTT_UNSUBSCRIBE,
    APP_STATE_MQTT_WAIT_FOR_ACTION,
    APP_STATE_MQTT_PUBLISH,
    APP_STATE_MQTT_PUBLISH_RESPOND,
    APP_STATE_END
} enAppState_t;

typedef enum {
    alpha = 97,
    speed = 115,
    rotation = 114
} channels_t;

/******************************************************
 *               Function Declarations
 ******************************************************/
static wiced_result_t mqtt_connection_event_cb(wiced_mqtt_object_t mqtt_object,wiced_mqtt_event_info_t *event);
static wiced_result_t mqtt_wait_for(wiced_mqtt_event_type_t event,uint32_t timeout);
static wiced_result_t mqtt_conn_open(wiced_mqtt_object_t mqtt_obj,wiced_ip_address_t *address, wiced_interface_t interface,
        wiced_mqtt_callback_t callback, wiced_mqtt_security_t *security);
static wiced_result_t mqtt_conn_close(wiced_mqtt_object_t mqtt_object);
static wiced_result_t mqtt_app_subscribe(wiced_mqtt_object_t mqtt_obj,char *topic, uint8_t qos);
static wiced_result_t mqtt_app_unsubscribe(wiced_mqtt_object_t mqtt_obj,char *topic);
static wiced_result_t mqtt_app_publish(wiced_mqtt_object_t mqtt_obj,uint8_t qos, char *topic, uint8_t *data, uint32_t data_len);
static wiced_result_t mqtt_start_wifi();
static wiced_mqtt_security_t mqtt_start_mqtt(wiced_mqtt_object_t mqtt_obj);

static void mqtt_stop_mqtt(wiced_mqtt_object_t mqtt_object,wiced_mqtt_security_t security);
static void mqtt_print_status(wiced_result_t result, const char * ok_message,const char *error_message);
/******************************************************
 *               Variable Definitions
 ******************************************************/
static wiced_ip_address_t broker_address;
static wiced_mqtt_callback_t callbacks = mqtt_connection_event_cb;
static wiced_mqtt_event_type_t expected_event;
static wiced_semaphore_t semaphore;
static char topic[100];


extern "C" void application_start();

#endif /** HOLONOMIC_FRAMEWORK **/
#endif /* RAMA_MQTT_HPP_ */
