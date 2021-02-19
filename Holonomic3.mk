#############################################
# Makefile for CYPRESS WICED WICED 43xxx ####
#############################################
# Include library folder in "libraries/drivers/sensors".
# Add lib to demo or snipped to the makefile
# variable $(NAME)_COMPONENTS += libraries/drivers/sensors

NAME := Lib_Holonomic3
GLOBAL_DEFINES   := HOLONOMIC_FRAMEWORK=HOLONOMIC_FRMWK_WICED

GLOBAL_INCLUDES  := src/config \
					src/framework/wiced-43xxx

$(NAME)_SOURCES  := src/Holonomic3.cpp

VALID_PLATFORMS := CYW943907AEVAL1F