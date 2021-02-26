/**
 * ============================================================================
 * \name        : Rama Killough a holonomic driving platform
 * \author      : Dr. Olaf Filies, Marcus Gordon Filies
 * \version     : 0.0.1
 * \copyright   : Infineon Technologies AG
 * \brief        Controlling 3 wheel holonomic platform
 * \details     : Controlling 3 wheel holonomic platform with
 *                one Infineon TLE94112EL microcontroller shield
 *                one Infineon XMC1100 or XMC4x00 microcontroller
 * used         : Infineon Arduino port for using XMC arm based microcontroller
 *                Infineon tle94112 base library
 *                1x Infineon XMC1100 microcontroller
 *                1x Infineon TLE94112 DC motor controller shield
 *
 * ============================================================================
 * Rama holonomic driving platform is on and waiting for input
 * ============================================================================
 * We need three values
 *     the angular alpha for the direction between 0deg-360deg
 *     the speed in alpha direction between 0%-100%
 *     the speed the rotation -100%-100% speed
 *         with negative values as anti clockwise
 *         and positive numbers for clockwise
 *     either alpha and alpha speed are set or rotation speed are set but
 *     yet not both
 * ============================================================================
 *
 * SPDX-License-Identifier: MIT
 */

#include <rama_mqtt.hpp>

#if (HOLONOMIC_FRAMEWORK == HOLONOMIC_FRMWK_WICED)


/******************************************************
 *               Pointer to motor controller
 ******************************************************/
Holonomic3 RamaDrive = Holonomic3();
Holonomic3::velInput_t driveParam = { 0, 0, 0 };

/**
 * @brief main loop function
 * 
 */
void application_start()
{
    static wiced_mqtt_object_t mqtt_object;
    wiced_mqtt_security_t security;
    wiced_result_t ret = WICED_SUCCESS;
    enAppState_t enAppState = APP_STATE_AFTER_RESET;
    uint8_t channel = 0;
    uint8_t value = 0;

    /* Main Loop */
    while (ret == WICED_SUCCESS) {
        switch (enAppState) {
        case APP_STATE_AFTER_RESET:
            wiced_init();
            /* Memory allocated for mqtt object*/
            mqtt_object = (wiced_mqtt_object_t) malloc(WICED_MQTT_OBJECT_MEMORY_SIZE_REQUIREMENT);
            if (mqtt_object == NULL) {
                WPRINT_APP_ERROR(("Dont have memory to allocate for mqtt object...\n"));
                return;
            }

            ret = mqtt_start_wifi();
            security = mqtt_start_mqtt(mqtt_object);

            WPRINT_APP_INFO(("[TLE94112] : setup begin -> %u\n", 0));
            RamaDrive.begin();
            RamaDrive.coastRama();
            WPRINT_APP_INFO(("[Holonomic drive] : startup begin -> %u\n", 0));

            enAppState = APP_STATE_MQTT_CONNECT;
            break;

        case APP_STATE_CONNECT_TO_WIFI:
            break;
        case APP_STATE_MQTT_FIND_BROKER:
            break;

        case APP_STATE_MQTT_CONNECT:
            WPRINT_APP_INFO(("[MQTT] Opening connection..."));
            RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR(
                 mqtt_conn_open( mqtt_object,&broker_address, WICED_STA_INTERFACE, callbacks,  &security ), NULL, "Did you configure you broker IP address?\n" );

            enAppState = APP_STATE_MQTT_SUBSCRIBE;
            break;

        case APP_STATE_MQTT_CLOSECONNECT:
            WPRINT_APP_INFO(("[MQTT] Closing connection..."));
            RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR(
                mqtt_conn_close(mqtt_object), NULL, NULL);

            enAppState = APP_STATE_END;
            break;

        case APP_STATE_MQTT_SUBSCRIBE:
            WPRINT_APP_INFO(("[MQTT] Subscribing..."));
            RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR(
                mqtt_app_subscribe( mqtt_object, WICED_TOPIC , WICED_MQTT_QOS_DELIVER_AT_MOST_ONCE ),NULL, NULL);

            enAppState = APP_STATE_MQTT_WAIT_FOR_ACTION;
            break;

        case APP_STATE_MQTT_UNSUBSCRIBE:
            WPRINT_APP_INFO(("[MQTT] Unsubscribing..."));
            RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR(
                mqtt_app_unsubscribe( mqtt_object, WICED_TOPIC ), NULL,NULL);

            enAppState = APP_STATE_MQTT_WAIT_FOR_ACTION;
            break;

        case APP_STATE_MQTT_PUBLISH: {
            WPRINT_APP_INFO(("[MQTT] Publishing..."));
            //RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR(
            //    mqtt_app_publish( mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, topic, (uint8_t*)valbuf ,sizeof(valbuf) ), NULL, NULL );

            channel++;

            enAppState = APP_STATE_MQTT_WAIT_FOR_ACTION;
            break;
        }

        case APP_STATE_MQTT_PUBLISH_RESPOND:
            WPRINT_APP_INFO(("[MQTT] Publish respond..."));

            enAppState = APP_STATE_MQTT_WAIT_FOR_ACTION;
            break;

        case APP_STATE_MQTT_WAIT_FOR_ACTION:

            WPRINT_APP_INFO( ("[MQTT] Waiting some time for ping exchange...\n\n"));
            wiced_rtos_delay_milliseconds(WICED_MQTT_DELAY_IN_MILLISECONDS * 1);

            value += WICED_MQTT_DELAY_IN_MILLISECONDS;
            channel = channel % 4;
            value = value % 255;

            enAppState = APP_STATE_MQTT_PUBLISH;
            break;

        case APP_STATE_END:
            break;

        default:
            while (1)
                ;
            break;

        } //switch (enAppState)
    }

    /* Stop Mqtt */
    mqtt_stop_mqtt(mqtt_object, security);
}

/******************************************************
 *               Static Function Definitions
 ******************************************************/

/**
 * @brief Wifi startup function
 *
 * @return wiced_result_t
 */
static wiced_result_t mqtt_start_wifi()
{
    WPRINT_APP_INFO(( "Starting Network...\n" ));
    wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);
    WPRINT_APP_INFO(( "Resolving IP address of MQTT broker...\n" ));
    wiced_result_t ret = wiced_hostname_lookup( MQTT_BROKER_ADDRESS, &broker_address, 10000, WICED_STA_INTERFACE);
    WPRINT_APP_INFO(
            ("Resolved Broker IP: %u.%u.%u.%u\n\n",
                (uint8_t)(GET_IPV4_ADDRESS(broker_address) >> 24),
                (uint8_t)(GET_IPV4_ADDRESS(broker_address) >> 16),
                (uint8_t)(GET_IPV4_ADDRESS(broker_address) >> 8),
                (uint8_t)(GET_IPV4_ADDRESS(broker_address) >> 0))
            );
    if (ret == WICED_ERROR || broker_address.ip.v4 == 0) {
        WPRINT_APP_INFO(("Error in resolving DNS\n"));
    }
    return ret;
}

/**
 * @brief MQTT startup function
 *
 * @param mqtt_object
 * @return wiced_mqtt_security_t
 */
static wiced_mqtt_security_t mqtt_start_mqtt(wiced_mqtt_object_t mqtt_object)
{
    uint32_t size_out;
    wiced_mqtt_security_t security;

    /* Read root CA certificate (self certified) from resources*/
    //resource_get_readonly_buffer(&resources_apps_DIR_secure_mqtt_DIR_ca_crt,     0, MQTT_MAX_RESOURCE_SIZE, &size_out,(const void **) &security.ca_cert);
    //security.ca_cert_len = size_out;
    //resource_get_readonly_buffer(&resources_apps_DIR_secure_mqtt_DIR_client_crt, 0, MQTT_MAX_RESOURCE_SIZE, &size_out,(const void **) &security.cert);
    //security.cert_len = size_out;
    resource_get_readonly_buffer(&resources_apps_DIR_secure_mqtt_DIR_client_key, 0, MQTT_MAX_RESOURCE_SIZE, &size_out,(const void **) &security.key);
    security.key_len = size_out;

    wiced_mqtt_init(mqtt_object);
    wiced_rtos_init_semaphore(&semaphore);

    return security;
}

/**
 * @brief MQTT stop function
 *
 * @param mqtt_object
 * @param security
 */
static void mqtt_stop_mqtt(wiced_mqtt_object_t mqtt_object, wiced_mqtt_security_t security)
{
    /* Stop Mqtt */
    wiced_rtos_deinit_semaphore(&semaphore);
    WPRINT_APP_INFO(("[MQTT] Deinit connection..."));
    wiced_result_t ret = wiced_mqtt_deinit(mqtt_object);
    mqtt_print_status(ret, NULL, NULL);
    free(mqtt_object);
    mqtt_object = NULL;

    /* Free security resources, only needed at initialization */
    //resource_free_readonly_buffer(&resources_apps_DIR_secure_mqtt_DIR_ca_crt,     security.ca_cert);
    //resource_free_readonly_buffer(&resources_apps_DIR_secure_mqtt_DIR_client_crt, security.cert);
    resource_free_readonly_buffer(&resources_apps_DIR_secure_mqtt_DIR_client_key, security.key);
}

/**
 * @brief A simple result log function
 *
 * @param result
 * @param ok_message
 * @param error_message
 */
static void mqtt_print_status(wiced_result_t result, const char * ok_message, const char * error_message)
{
    if (result == WICED_SUCCESS) {
        if (ok_message != NULL) {
            WPRINT_APP_INFO(( "OK (%s)\n\n", (ok_message)));
        } else {
            WPRINT_APP_INFO(( "OK.\n\n" ));
        }
    } else {
        if (error_message != NULL) {
            WPRINT_APP_INFO(( "ERROR (%s)\n\n", (error_message)));
        } else {
            WPRINT_APP_INFO(( "ERROR.\n\n" ));
        }
    }
}

/**
 * @brief Call back function to handle connection events.
 *
 * @param mqtt_object
 * @param event
 * @return wiced_result_t
 */
static wiced_result_t mqtt_connection_event_cb(wiced_mqtt_object_t mqtt_object, wiced_mqtt_event_info_t *event)
{
    switch (event->type) {

    case WICED_MQTT_EVENT_TYPE_CONNECT_REQ_STATUS:
    case WICED_MQTT_EVENT_TYPE_DISCONNECTED:
    case WICED_MQTT_EVENT_TYPE_PUBLISHED:
    case WICED_MQTT_EVENT_TYPE_SUBSCRIBED:
    case WICED_MQTT_EVENT_TYPE_UNSUBSCRIBED: {
            expected_event = event->type;
            wiced_rtos_set_semaphore(&semaphore);
        }
        break;

    case WICED_MQTT_EVENT_TYPE_PUBLISH_MSG_RECEIVED: {
            wiced_mqtt_topic_msg_t msg = event->data.pub_recvd;

            int value = 0;
            for (int i = 0; i < (int) msg.data_len; i++){
                value = 10 * value + (msg.data[i] - '0');
            }
            switch (msg.topic[6]){
                case alpha:{
                    driveParam.alpha = value;
                    break;
                }
                case speed:{
                    driveParam.speed = value;
                    break;
                }
                case rotation:{
                    driveParam.rotation = value;
                    break;
                }
            }
            if (driveParam.speed != 0) {
                RamaDrive.driveXY(driveParam);
            } else {
                if (driveParam.rotation == 0) {
                    RamaDrive.coastRama();
                } else {
                    RamaDrive.driveRot(driveParam);
                }
            }
            WPRINT_APP_INFO( ( "[MQTT] TOPIC : %.*s\n", (int) msg.topic_len, msg.topic));
            WPRINT_APP_INFO( ( "[MQTT] DATA  : %.*s,%u\n", (int) msg.data_len, msg.data,(int) msg.data_len ));
            WPRINT_APP_INFO( ("OK -> <%u,%u,%u>\n", driveParam.alpha,driveParam.speed,driveParam.rotation));

        }
        break;

    default:
        break;
    }

    return WICED_SUCCESS;
}

/**
 * Call back function to handle channel events.
 *
 * For each event:
 *  - The call back will set the expected_event global to the received event.
 *  - The call back will set the event semaphore to run any blocking thread functions waiting on this event
 *  - Some events will also log other global variables required for extra processing.
 *
 * A thread function will probably be waiting for the received event. Once the event is received and the
 * semaphore is set, the thread function will check for the received event and make sure it matches what
 * it is expecting.
 *
 * Note:  This mechanism is not thread safe as we are using a non protected global variable for read/write.
 * However as this snip is a single controlled thread, there is no risc of racing conditions. It is
 * however not recommended for multi-threaded applications.
 */


/**
 * @brief A blocking call to an expected event.
 *
 * @param event
 * @param timeout
 * @return wiced_result_t
 */
static wiced_result_t mqtt_wait_for(wiced_mqtt_event_type_t event, uint32_t timeout)
{
    if (wiced_rtos_get_semaphore(&semaphore, timeout) != WICED_SUCCESS) {
        return WICED_ERROR;
    } else {
        if (event != expected_event) {
            return WICED_ERROR;
        }
    }
    return WICED_SUCCESS;
}

/**
 * @brief Open a connection and wait for WICED_MQTT_TIMEOUT period to receive a connection open OK event
 *
 * @param mqtt_obj
 * @param address
 * @param interface
 * @param callback
 * @param security
 * @return wiced_result_t
 */
static wiced_result_t mqtt_conn_open(wiced_mqtt_object_t mqtt_obj, wiced_ip_address_t *address, wiced_interface_t interface, wiced_mqtt_callback_t callback, wiced_mqtt_security_t *security)
{
    wiced_mqtt_pkt_connect_t conninfo;
    wiced_result_t ret = WICED_SUCCESS;

    memset(&conninfo, 0, sizeof(conninfo));

    conninfo.port_number = 0; /* set to 0 indicates library to use default settings */
    conninfo.mqtt_version = WICED_MQTT_PROTOCOL_VER3;
    conninfo.clean_session = 1;
    conninfo.client_id = (uint8_t*) MQTT_BROKER_CLIENT_ID;
    conninfo.keep_alive = 5;
    conninfo.password = (uint8_t*) MQTT_BROKER_USER_NAME;
    conninfo.username = (uint8_t*) MQTT_BROKER_USER_PASSWORD;
    conninfo.peer_cn = NULL;

    ret = wiced_mqtt_connect(mqtt_obj, address, interface, callback, security,
            &conninfo);
    if (ret != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    if (mqtt_wait_for(WICED_MQTT_EVENT_TYPE_CONNECT_REQ_STATUS,WICED_MQTT_TIMEOUT) != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}

/**
 * @brief Close a connection and wait for 5 seconds to receive a connection close OK event
 *
 * @param mqtt_obj
 * @return wiced_result_t
 */
static wiced_result_t mqtt_conn_close(wiced_mqtt_object_t mqtt_obj)
{
    if (wiced_mqtt_disconnect(mqtt_obj) != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    if (mqtt_wait_for(WICED_MQTT_EVENT_TYPE_DISCONNECTED, WICED_MQTT_TIMEOUT) != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}

/**
 * @brief Subscribe to WICED_TOPIC and wait for 5 seconds to receive an ACM.
 *
 * @param mqtt_obj
 * @param topic
 * @param qos
 * @return wiced_result_t
 */
static wiced_result_t mqtt_app_subscribe(wiced_mqtt_object_t mqtt_obj,char *topic, uint8_t qos)
{
    wiced_mqtt_msgid_t pktid;
    pktid = wiced_mqtt_subscribe(mqtt_obj, topic, qos);
    if (pktid == 0) {
        return WICED_ERROR;
    }
    if (mqtt_wait_for(WICED_MQTT_EVENT_TYPE_SUBSCRIBED, WICED_MQTT_TIMEOUT) != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}

/**
 * @brief Unsubscribe from WICED_TOPIC and wait for 10 seconds to receive an ACM.
 *
 * @param mqtt_obj
 * @param topic
 * @return wiced_result_t
 */
static wiced_result_t mqtt_app_unsubscribe(wiced_mqtt_object_t mqtt_obj,char *topic)
{
    wiced_mqtt_msgid_t pktid;
    pktid = wiced_mqtt_unsubscribe(mqtt_obj, topic);

    if (pktid == 0) {
        return WICED_ERROR;
    }
    if (mqtt_wait_for(WICED_MQTT_EVENT_TYPE_UNSUBSCRIBED, WICED_MQTT_TIMEOUT * 2) != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}

/**
 * @brief Publish (send) WICED_MESSAGE_STR to WICED_TOPIC and wait for 5 seconds to receive a PUBCOMP (as it is QoS=2).
 *
 * @param mqtt_obj
 * @param qos
 * @param topic
 * @param data
 * @param data_len
 * @return wiced_result_t
 */
static wiced_result_t mqtt_app_publish(wiced_mqtt_object_t mqtt_obj,uint8_t qos, char *topic, uint8_t *data, uint32_t data_len)
{
    wiced_mqtt_msgid_t pktid;
    pktid = wiced_mqtt_publish(mqtt_obj, topic, data, data_len, qos);

    if (pktid == 0) {
        return WICED_ERROR;
    }

    if (mqtt_wait_for(WICED_MQTT_EVENT_TYPE_PUBLISHED, WICED_MQTT_TIMEOUT) != WICED_SUCCESS) {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}

#endif
