/**
 * ============================================================================
 * \name        : Rama Killough a holonomic driving platform
 * \author      : Dr. Olaf Filies, Marcus Gordon Filies
 * \version     : 0.0.1
 * \copyright   : Infineon Technologies AG
 * \brief       : Controlling 3 wheel holonomic platform with
 * \brief       : Controlling 3 wheel holonomic platform with
 *                one Infineon TLE94112EL microcontroller shield
 *                one Infineon XMC1100 or XMC4x00 microcontroller
 * used         : Infineon Arduino port for using XMC arm based microcontroller
 *                Infineon tle94112 base library
 *                1x Infineon XMC1100 microcontroller
 *                1x Infineon TLE94112 DC motor controller shield
 * ============================================================================
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
 */

#ifndef RAMA_HPP_
#define RAMA_HPP_

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
#include "wiced_rtos.h"

extern "C" void application_start();

#endif /** HOLONOMIC_FRAMEWORK **/
#endif /* RAMA_HPP_ */
