/*!
 * ============================================================================
 * \name         Rama Killough a holonomic driving platform with three motors
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

#ifndef HOLONOMIC3_HPP
#define HOLONOMIC3_HPP
#include "config/holonomic-conf.hpp"

//!< Motors clearable errors (OPENLOAD and UNDER/OVERCURRENT)
#define IGNORE_ERROR

/**
 * @addtogroup holoCore
 * @{
 *
 * @brief Core library for holonomic driving platform
 *
 * This class provides a simple API for holonomic driving with the TLE94112 shield
 * and three motors using four halfbridges each.
 */

#if (HOLONOMIC_FRAMEWORK == HOLONOMIC_FRMWK_ARDUINO)
#include "framework/arduino/Holonomic3-ino.hpp"
#elif (HOLONOMIC_FRAMEWORK == HOLONOMIC_FRMWK_WICED)
#include "framework/wiced-43xxx/Holonomic3-wiced.hpp"
#endif /* HOLONOMIC_FRAMEWORK */

class Holonomic3: virtual public Holonomic3Pal
{
	public:
		/*!
		* Velocity Input data for rama robot, which is:
		*  alpha      the angle of driving direction according the rama robots 0deg
		*  speed      the speed in angle direction
		*  rotation   the rotation speed
		*/
		typedef struct velInput
		{
			int	alpha;
			int speed;
			int rotation;
		} velInput_t;

		/*!
		* the calculated direction vector for x/y/r
		*/
		typedef struct velVector
		{
			float x;
			float y;
			float r;
		} velVector_t;

		/*!
		* the motor speed factor for each of the three motors.
		* This factor has to be multiplied by 255 the max. motor speed
		*/
		typedef struct motorSpeed
		{
			float ms1;
			float ms2;
			float ms3;
		} motorSpeed_t;


		Holonomic3();
		~Holonomic3();

		/*!
		* \brief Initialize all three motors
		*
		* Initialize all motors for the rama. As we are using three motors
		* on the TLE94112 each on two half bridges and its own PWM signal,
		* we where able to control each motor with its own speed. This is
		* needed for a proper function of the robot.
		*/
		void begin();

		/*!
		* \brief Coasts all motors at the end to smoothly switch of rama robot
		*/
		void end(void);

		/*!
		* \brief Function stops all motors and holds there position with given force
		*        or with the maximal force by default if nothing is inserted
		*/
		void coastRama();

		/*!
		* \brief Stop all motors with coast command, no voltage on the open half bridges
		*        and active free wheeling
		*        clears all errors at the end
		*
		* @param force the motor holding force from 0 - 255 default max 255.
		*/
		void stopRama(uint8_t force=255);

		/*!
		* \brief Drive simple function for only x/y direction
		*
		* Function converts input velocity into the velocity factors
		* for x,y,r direction and calculates the speed factor for each motor
		* before it runs the motors.
		*
		* It clears all errors before it starts
		*
		* @param hInput Input velocity form outside controller
		*/
		void driveRama(velInput_t hInput);

		/*!
		* \brief Drives the rama robot only in x/y direction without rotation
		*
		* For driving the rama robot only with two vectors in x/y direction
		* the rotation r has to set to 0. Also the calculated speed factor
		* can be raised about 1/3 as one vector is eliminated.
		*
		* It clears all errors before it starts
		*
		* @param xyInput the input values with alpha, speed and rotation=0
		*/
		void driveXY(velInput_t xyInput);

		/*!
		* \brief Rotates rama robot by running all motors in same direction with same speed
		*
		* To rotate the rama robot we set the input speed factor
		* to 0 so that we only have the rotation speed.
		*
		* It clears all errors before it starts
		*
		* @param rInput Inserts the rotation speed
		*/
		void driveRot(velInput_t rInput);

		/*!
		* \brief Calculates the speed vectors for motor 1 to 3
		*
		* calcSpeedVector function calculates
		* - the x/y vector from the input alpha angle in radians
		* - maps the rotation speed between -1 and 1
		* - calculates with the inverse velocity matrix P0 the motor speed for each motor
		* - normalize the motor speed factors with the speed in alpha direction
		*
		* @param  hInput  the input values
		* @return mFactor the calculated motor speed factors
		*/
		motorSpeed_t calcSpeedVector(velInput_t hInput);

		/*!
		* \brief Returns the calculated actual motor speed factor
		* @return mFactor the motor speed factors
		*/
		motorSpeed_t getSpeedFactor(void);

		/*!
		* \brief Returns the actual! set driving speed of each motor
		* @return motorSpeed_t structure
		*/
		motorSpeed_t getSpeed(void);

		/*!
		* \brief Returns the calculated x/y/r direction vector
		* @return mVector the direction vector
		*/
		velVector_t  getVector(void);

	protected:

		/*!
		* Rama three motor structure setting
		* using TLE94112 DC motor controller shield
		*/
		typedef struct holoMotor
		{
			Tle94112Motor motor1;
			Tle94112Motor motor2;
			Tle94112Motor motor3;
		} holoMotor_t;

		/*!
		* The three rama motors each on its own PWM and
		* connected to two half bridges
		*/
		holoMotor_t rama {
			{controller},
			{controller},
			{controller}
		};

		/*!
		* for fast deg to rad conversion we will us
		* the preset value of 1 deg
		*/
		float rad = 0.017453;

		/*!
		* We need the inverse kinematics matrix P0 for a holonomic drive with
		* three wheels at position
		*  motor1 = 30deg
		*  motor2 = 150deg
		*  motor3 = 270deg
		* We want to obtain the velocity for x,y,r.
		* TODO implement inverse matrix with analytic function for in drive rotation.
		* Therefore the hard motor positions 30/150/270deg must be rotated
		*/
		float P0[3][3] = {
			{ -0.333,  0.577, 0.333 },
			{ -0.333, -0.577, 0.333 },
			{  0.666,    0.0, 0.333 }
		};

		//! the input vector with angle, speed and direction
		velInput_t   mInput   { 0, 0, 0 };

		//! the calculated direction vector
		velVector_t  mVector  { 0, 0, 0 };

		//! the calculated motor speed vector
		motorSpeed_t mFactor  { 0, 0, 0 };

};
/** @} */

#endif /* HOLONOMIC3_HPP */
