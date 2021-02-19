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
 *
 * ============================================================================
 * "A rama does everything by count of three"
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "Holonomic3.hpp"


Holonomic3::Holonomic3()
{
}

Holonomic3::~Holonomic3()
{
	end();
}

void Holonomic3::begin()
{
	controller.begin();

	// motor1 init connected to first four half bridges
	rama.motor1.initConnector(rama.motor1.HIGHSIDE,controller.TLE_NOPWM,controller.TLE_FREQ200HZ,controller.TLE_HB1, controller.TLE_HB2, controller.TLE_NOHB,controller.TLE_NOHB);
	rama.motor1.initConnector(rama.motor1.LOWSIDE, controller.TLE_PWM1, controller.TLE_FREQ200HZ,controller.TLE_HB3, controller.TLE_HB4, controller.TLE_NOHB,controller.TLE_NOHB);

	// motor2 init connected to second four half bridges
	rama.motor2.initConnector(rama.motor2.HIGHSIDE,controller.TLE_NOPWM,controller.TLE_FREQ200HZ,controller.TLE_HB5, controller.TLE_HB6, controller.TLE_NOHB,controller.TLE_NOHB);
	rama.motor2.initConnector(rama.motor2.LOWSIDE, controller.TLE_PWM2, controller.TLE_FREQ200HZ,controller.TLE_HB7, controller.TLE_HB8, controller.TLE_NOHB,controller.TLE_NOHB);

	// motor3 init connected to third four half bridges
	rama.motor3.initConnector(rama.motor3.HIGHSIDE,controller.TLE_NOPWM,controller.TLE_FREQ200HZ,controller.TLE_HB9, controller.TLE_HB10,controller.TLE_NOHB,controller.TLE_NOHB);
	rama.motor3.initConnector(rama.motor3.LOWSIDE, controller.TLE_PWM3, controller.TLE_FREQ200HZ,controller.TLE_HB11,controller.TLE_HB12,controller.TLE_NOHB,controller.TLE_NOHB);

	//call motors
	rama.motor1.begin();
	rama.motor2.begin();
	rama.motor3.begin();
}

void Holonomic3::end(void)
{
	rama.motor1.coast();
	rama.motor2.coast();
	rama.motor3.coast();
}

void Holonomic3::stopRama(uint8_t force)
{
	rama.motor1.stop(force);
	rama.motor2.stop(force);
	rama.motor3.stop(force);
}

void Holonomic3::coastRama()
{
	rama.motor1.coast();
	rama.motor2.coast();
	rama.motor3.coast();
#ifdef IGNORE_ERROR
	controller.clearErrors();
	controller.clearErrors();
#endif
}

void Holonomic3::driveRama(velInput_t hInput)
{
#ifdef IGNORE_ERROR
	controller.clearErrors();
#endif
	mFactor = calcSpeedVector(hInput);
	rama.motor1.start(int(255 * mFactor.ms1));
	rama.motor2.start(int(255 * mFactor.ms2));
	rama.motor3.start(int(255 * mFactor.ms3));
}

void Holonomic3::driveXY(velInput_t xyInput)
{
#ifdef IGNORE_ERROR
	controller.clearErrors();
#endif
	xyInput.rotation = 0; // set rotation definitely to 0
	mFactor = calcSpeedVector(xyInput);
	rama.motor1.start(int(255 * mFactor.ms1 * 1.5));
	rama.motor2.start(int(255 * mFactor.ms2 * 1.5));
	rama.motor3.start(int(255 * mFactor.ms3 * 1.5));
}

void Holonomic3::driveRot(velInput_t rInput)
{
	// rotation speed as factor -1 to 1
	mVector.r = constrain( rInput.rotation * 0.01 , -1, 1);
	rama.motor1.start( int( 255 * mVector.r ) );
	rama.motor2.start( int( 255 * mVector.r ) );
	rama.motor3.start( int( 255 * mVector.r ) );
#ifdef IGNORE_ERROR
	controller.clearErrors();
#endif
}

Holonomic3::motorSpeed_t Holonomic3::getSpeed(void)
{
	motorSpeed_t maFactor  {
		rama.motor1.getSpeed(),
		rama.motor2.getSpeed(),
		rama.motor3.getSpeed()
	};
	return (maFactor);
}

Holonomic3::velVector_t Holonomic3::getVector(void)
{
	return (mVector);
}

Holonomic3::motorSpeed_t Holonomic3::getSpeedFactor(void)
{
	return (mFactor);
}

Holonomic3::motorSpeed_t Holonomic3::calcSpeedVector(velInput_t hInput)
{
	// save input value
	mInput = hInput;

	// rotation speed as factor -1 to 1
	mVector.r = constrain( hInput.rotation * 0.01 , -1, 1);
	// calculate speed factor max -1 to 1
	float speed = hInput.speed * 0.01;

	// calculate x/y vector from angle alpha
	if (hInput.speed == 0) {
		mVector.x = 0;
		mVector.y = 0;
	}else{
		float alpha_rad = hInput.alpha * rad;
		mVector.x = cos( alpha_rad );
		mVector.y = sin( alpha_rad );
	}

	mFactor.ms1 = ( P0[0][0] * mVector.x + P0[0][1] * mVector.y + P0[0][2] * mVector.r ) * speed;
	mFactor.ms2 = ( P0[1][0] * mVector.x + P0[1][1] * mVector.y + P0[1][2] * mVector.r ) * speed;
	mFactor.ms3 = ( P0[2][0] * mVector.x + P0[2][1] * mVector.y + P0[2][2] * mVector.r ) * speed;

	return (mFactor);
}

