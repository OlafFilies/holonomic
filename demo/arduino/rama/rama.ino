/*
 ============================================================================
 \name        : Lego Rama Killough a holonomic driving platform
 \author      : Dr. Olaf Filies, Marcus Gordon Filies
 \version     : 0.0.1
 \copyright   : Infineon Technologies AG
 \brief       : Controlling 3 wheel holonomic platform with
                one Infineon TLE94112EL microcontroller shield
                one Infineon XMC1100 or XMC4x00 microcontroller
 used         : Infineon Arduino port for using XMC arm based microcontroller
                Infineon tle94112 base library
                1x Infineon XMC1100 microcontroller
                1x Infineon TLE94112 DC motor controller shield
 ============================================================================
 
 ============================================================================
 Rama holonomic driving platform is on and waiting for input
 ============================================================================
 We need three values
     the angular alpha for the direction between 0deg-360deg
     the speed in alpha direction between 0%-100%
     the speed the rotation -100%-100% speed
         with negative values as anti clockwise
         and positive numbers for clockwise
     either alpha and alpha speed are set or rotation speed are set but
     yet not both
 ============================================================================
 */

#include <Holonomic3.hpp>

Holonomic3 RamaDrive = Holonomic3();
Holonomic3::velInput_t driveParam = { 0, 0, 0};

void setup() {
  RamaDrive.begin();
  RamaDrive.coastRama();
  delay(500);
  
  Serial.begin(38400);
  while (!Serial) {}
  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    Serial.readStringUntil('<');
    driveParam.alpha    = Serial.readStringUntil(',').toInt();
    driveParam.speed    = Serial.readStringUntil(',').toInt();
    driveParam.rotation = Serial.readStringUntil('>').toInt();
    if (driveParam.speed != 0) {
      RamaDrive.driveXY(driveParam);
    }else{
      if (driveParam.rotation == 0) {
        RamaDrive.coastRama();
      } else {
        RamaDrive.driveRot(driveParam);
      }
    }
    Serial.println("Ok");
    Serial.print("<");Serial.print(driveParam.alpha);
    Serial.print(",");Serial.print(driveParam.speed);
    Serial.print(",");Serial.print(driveParam.rotation);
    Serial.print(">");
  }
}
