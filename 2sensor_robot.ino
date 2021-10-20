#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(4);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(3);
const uint8_t IR_sensor_left = 0; // for incoming serial data
const uint8_t IR_sensor_right = 2; // for incoming serial data

const uint8_t CMD_BUFFER_LEN = 128;
uint16_t cmd_buffer_pos = 0;
char cmd_buffer[CMD_BUFFER_LEN];
bool str2hex(char *str, uint16_t *val);
int data_left;
int data_right;
int RightMotorDirection; //0 if backwards 1 if forwards
int LeftMotorDirection; //0 if backwards 1 if forwards
int MotorSpeed = 0;
int SensorThreshold = 1000;

void setup() {
AFMS.begin();
    pinMode(IR_sensor_left, INPUT); //initialize left ldr sensor as INPUT
     pinMode(IR_sensor_right, INPUT); //initialize right ldr sensor as INPUT
    Serial.begin(115200); //begin the serial monitor at 115200 baud
    
}

void loop() {
  // put your main code here, to run repeatedly:
  
RightMotor->setSpeed(MotorSpeed);
LeftMotor->setSpeed(MotorSpeed);
data_left=analogRead(IR_sensor_left);
data_right=analogRead(IR_sensor_right);

if (data_left<SensorThreshold && data_right<SensorThreshold){
  RightMotor->run(FORWARD);
  LeftMotor->run(BACKWARD);
  RightMotorDirection=0;
  LeftMotorDirection=1;
}
else if (data_left>=SensorThreshold){

    RightMotor->run(FORWARD);
  LeftMotor->run(FORWARD);
  RightMotorDirection=0;
  LeftMotorDirection=0;
}
else if (data_right>=SensorThreshold){
  RightMotor->run(BACKWARD);
  LeftMotor->run(BACKWARD);
  RightMotorDirection=1;
  LeftMotorDirection=1;
}
    
if (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\r') {
      cmd_buffer[cmd_buffer_pos] = '\0';
      parse_cmd_buffer();
      cmd_buffer_pos = 0;
    } else if (cmd_buffer_pos == CMD_BUFFER_LEN - 1) {
      cmd_buffer_pos = 0;
      cmd_buffer[cmd_buffer_pos] = ch;
      cmd_buffer_pos++;
    } else {
      cmd_buffer[cmd_buffer_pos] = ch;
      cmd_buffer_pos++;
    }
  }
}
    
void parse_cmd_buffer() {
  uint16_t val;
  if (strcmp(cmd_buffer, "LeftSensor?") == 0) {
    Serial.print(data_left, HEX);
    Serial.print("\r\n");
  } 
  else if (strcmp(cmd_buffer, "RightSensor?") == 0) {
    Serial.print(data_right, HEX);
    Serial.print("\r\n");
  }
  else if (strncmp(cmd_buffer, "MotorSpeed!", 11) == 0) {
    if (str2hex(cmd_buffer + 11, &val)) {
      MotorSpeed = val;
    }
  } 
  else if (strcmp(cmd_buffer, "MotorSpeed?") == 0) {
    Serial.print(MotorSpeed, HEX);
    Serial.print("\r\n");
  }
  else if (strncmp(cmd_buffer, "SensorThreshold!", 16) == 0) {
    if (str2hex(cmd_buffer + 16, &val)) {
      SensorThreshold = val;
    }
  } 
  else if (strcmp(cmd_buffer, "SensorThreshold?") == 0) {
    Serial.print(SensorThreshold, HEX);
    Serial.print("\r\n");
  }
  else if (strcmp(cmd_buffer, "RightMotorDirection?") == 0) {
    Serial.print(RightMotorDirection, HEX);
    Serial.print("\r\n");
  }
  else if (strcmp(cmd_buffer, "LeftMotorDirection?") == 0) {
    Serial.print(LeftMotorDirection, HEX);
    Serial.print("\r\n");
  }
}

/*
** Converts a null-terminated (C-style) string that is pointed to by str 
** into a 16-bit unsigned integer that is returned using call by reference 
** via val, skipping over any initial space or tab characters.  The function 
** returns a boolean value that is true if the conversion succeeded.  The 
** conversion is considered failed if there is no valid sequence of hex 
** digits or if the sequence of hex digits does not end with the end of 
** the string.
*/
bool str2hex(char *str, uint16_t *val) {
  uint8_t pos = 0;

  if (str[pos] == '\0')
    return false;

  while ((str[pos] == ' ') || (str[pos] == '\t')) {
    pos++;
  }

  *val = 0;
  while (str[pos] != '\0') {
    if ((str[pos] >= '0') && (str[pos] <= '9')) {
      *val = (*val << 4) + (str[pos] - '0');
    } else if ((str[pos] >= 'a') && (str[pos] <= 'f')) {
      *val = (*val << 4) + 10 + (str[pos] - 'a');
    } else if ((str[pos] >= 'A') && (str[pos] <= 'F')) {
      *val = (*val << 4) + 10 + (str[pos] - 'A');
    } else {
      return false;
    }
    pos++;
  }

  return true;
}
