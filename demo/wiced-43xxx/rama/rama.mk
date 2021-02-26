NAME := App_rama

$(NAME)_SOURCES := rama.cpp

$(NAME)_INCLUDES += ./

$(NAME)_COMPONENTS := drivers/motor-control/Holonomic3

VALID_PLATFORMS	:= CYW943907AEVAL1F