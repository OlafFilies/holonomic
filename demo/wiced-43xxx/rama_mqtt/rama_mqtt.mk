
NAME := App_rama_mqtt

$(NAME)_SOURCES := rama_mqtt.cpp

$(NAME)_INCLUDES += ./

$(NAME)_COMPONENTS := protocols/MQTT \
                      drivers/motor-control/Holonomic3

$(NAME)_RESOURCES := apps/secure_mqtt/situla/client.key \
                     apps/secure_mqtt/situla/client.crt \
                     apps/secure_mqtt/situla/ca.crt

$(NAME)_CFLAGS += -xC++

WIFI_CONFIG_DCT_H := wifi_config_dct.h


VALID_PLATFORMS	:= CYW943907AEVAL1F

ifeq ($(BUILD_TYPE),debug)
    GLOBAL_DEFINES += APPLICATION_STACK_SIZE=8196
endif
