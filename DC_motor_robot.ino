#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
void setup() {
  // put your setup code here, to run once:
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);
AFMS.begin();
LeftMotor->setSpeed(150);
LeftMotor->run(BACKWARD);
RightMotor->setSpeed(150);
RightMotor->run(BACKWARD);

}

void loop() {
  // put your main code here, to run repeatedly:


}
