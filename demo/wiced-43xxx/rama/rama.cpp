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

#include <rama.hpp>

#if (HOLONOMIC_FRAMEWORK == HOLONOMIC_FRMWK_WICED)

//!< Max input legth for number of three digits
#define NAME_LENGTH     3

Holonomic3 RamaDrive = Holonomic3();
Holonomic3::velInput_t driveParam = {0, 0, 0};

/**
 * @brief setup function
 * 
 */
void setup()
{
    wiced_core_init();
    /* Remove buffering from all std streams */
    setvbuf( stdin, NULL, _IONBF, 0 );
    setvbuf( stdout, NULL, _IONBF, 0 );
    setvbuf( stderr, NULL, _IONBF, 0 );

    // Enable MotorController on TLE94112 shields and cost all motors
    WPRINT_APP_INFO(("[TLE94112] : setup begin -> %u\n", 0));
    RamaDrive.begin();
    RamaDrive.coastRama();
    WPRINT_APP_INFO(("[Holonomic drive] : startup begin -> %u\n", 1));
    WPRINT_APP_INFO(("Insert -> <alpha,speed,rotation>\n"));
}

/**
 * @brief loop function
 * 
 */
void loop()
{
    char ch;
    int value = 0;
    int param_index = 0;

    while(1) {
        ch = getchar();
        if (ch >= '0' && ch <= '9') {
            value = 10 * value + (ch - '0');
        } else if ((ch == ',' || ch == ';' || ch == '>')) {
            WPRINT_APP_INFO(("value %u; indx %u\n", value,param_index));
            switch(param_index) {
                case 0: driveParam.alpha = value;
                case 1: driveParam.speed = value;
                case 2: driveParam.rotation = value;
            }
            value = 0;
            param_index++;
        } else if ((ch == '\n')) {
            value = 0;
            param_index = 0;

            if (driveParam.speed != 0) {
                RamaDrive.driveXY(driveParam);
            } else {
                if (driveParam.rotation == 0) {
                    RamaDrive.coastRama();
                } else {
                    RamaDrive.driveRot(driveParam);
                }
            }
            WPRINT_APP_INFO(("OK -> <%u,%u,%u>\n", driveParam.alpha,driveParam.speed,driveParam.rotation));
        }
    }
}

/**
 * @brief main loop function
 * 
 */
void application_start( )
{
    wiced_result_t result;

    /* Initialize the device */
    result = wiced_init();
    if (result != WICED_SUCCESS)
    {
        return;
    }

    setup();

    while(1)
    {
        loop();
    }
}

#endif
