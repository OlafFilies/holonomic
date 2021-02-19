## holonomic

> Holonomic driving with Infineon XMC micro controller and TLE94112 motor controller


## What it is

This library drives a three motor holonomic platform, a rama killough, with omniwheels.
This platform can drive in x/y direction or rotate directly on site.

## Features

- Initiates all three motors virtually placed at 30°,150° and 270° 
- runs motors by calculating the driving speed of each motor for the given angular
- rotates the platform by driving all motors three motors in the same direction
- controls the speed of the whole platform
- coasts or stops the platform
- is based on the Infineon TLE94112 motor controller library
- runs best with Infineon XMC1100 or XMC4700 micro controller

## Demo platform

A LEGO(R) based demo platform for a rama killough is available

## Hardware needed

For this project you need at least the following parts:

- an Infineon XMC micro controller like the XMC1100 or a XMC4700 and compatible
- an Infineon TLE94112 DC motor controller
- three Lego(R) EV3 or NXT motors
- 6 Omniwheels with Lego(R) compatible hub
- Lego lever and pins, batteries box etc. (see building plan)
- a voltage regulator with 6-12V input and 5V output to power the XMC microc ontroller with the 9V batteries box
- a Bluetooth or wifi connection for communication

## Software needed

For the base robot software you need:

- Infineon XMC Arduino port
- the TLE94112 DC motor library
- this library
- optional Bluetooth or wifi software ports
- optional Infineon Optiga Trust for secure communication
