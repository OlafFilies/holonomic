

#ifndef MQTT_CONFIG_H_
#define MQTT_CONFIG_H_


/******************************************************
 *                    Constants
 ******************************************************/
/* Change the Broker domain address to match the server address */
#define MQTT_BROKER_ADDRESS                 "YourBrokerIP"
/* Name of topic name */
#define WICED_TOPIC                         "robot/#"
/* MQTT message content */
#define WICED_MESSAGE_STR                   "Holonomic roboter with WICED"
/* MQTT client ID */
#define MQTT_BROKER_CLIENT_ID               "wiced"
/* MQTT Timeout settings */
#define WICED_MQTT_TIMEOUT                  (10000)
/* MQTT delay settings */
#define WICED_MQTT_DELAY_IN_MILLISECONDS    (1000)
/* Max resource size */
#define MQTT_MAX_RESOURCE_SIZE              (0x7fffffff)
/* MQTT username */
#define MQTT_BROKER_USER_NAME               "Your Broker user name"
/* MQTT passwd */
#define MQTT_BROKER_USER_PASSWORD           "Your Password"


#endif /* MQTT_CONFIG_H_ */
