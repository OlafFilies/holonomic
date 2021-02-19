/*!
 * ============================================================================
 * \name         Rama Killough a holonomic driving platform
 * \author       Dr. Olaf Filies, Marcus Gordon Filies
 * \version      1.0.1
 * \copyright    Infineon Technologies AG
 *
 * \brief        Controlling 3 wheel holonomic platform
 * \details      This class runs a rama_killough, a 3 wheel holonomic platform,
 *               which means it can run in any x,y direction at once and turn
 *               around itself.
 *               To run this class you need:
 *               - a rama killough holonomic driving platform
 *               - an Arduino or compatible micro controller like Infineons XMC1100 or XMC4700
 *               - an Infineon TLE94112 DC Motorcontroller Shield
 *
 * ============================================================================
 * "A rama does everything by count of three"
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef HOLONOMICINO_HPP
#define HOLONOMICINO_HPP

#include "config/holonomic-conf.hpp"

#if (HOLONOMIC_FRAMEWORK == HOLONOMIC_FRMWK_ARDUINO)
#include <TLE94112-ino.hpp>
#include <TLE94112Motor-ino.hpp>


/**
 * @addtogroup palIno
 * @{
 *
 * @brief Arduino Hardware Platform
 *
 * This is a simple wrapper for the arduino framework, which only loads ths
 * right header and set the right controller.

 */

class Holonomic3Pal
{

	protected:
		//! Tle94112 Object
		Tle94112Ino controller = Tle94112Ino();

};
/** @} */

#endif /* HOLONOMIC_FRAMEWORK */
#endif /* HOLONOMICINO_HPP */
