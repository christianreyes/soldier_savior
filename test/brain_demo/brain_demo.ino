#include <SoftwareSerial.h>
#include "SerialCommand.h"

//-------------------------------------------------------------- define global variables --------------------------------------------

unsigned long permission = 10000UL;
int LeftPWM = 0;                                                  // PWM value for left  motor speed / brake
int RightPWM = 0;                                                 // PWM value for right motor speed / brake

//-------------------------------------------------------------- define GPS ------------------------------------------------------

SoftwareSerial ss_vehicle(NULL, 9); // RX, TX
SerialCommand sCmd;

boolean toggle = true;

void setup() {
  pinMode(13, OUTPUT);
  
  ss_vehicle.begin(9600);  // start vehicle communication  
  Serial.begin(9600);  // start vehicle communication  
  
  Serial.println(" ");

  sCmd.addCommand("GO", go);

  delay(1000);             // wait for vehicle systems to start
}

void loop() {
  sCmd.readSerial();
  
  if(permission < 2UL ){
    ss_vehicle.println("SETSPEED 158 200");
    digitalWrite(13, HIGH);
  } else {
    ss_vehicle.println("SETSPEED 0 0");
    //delay(1);
    digitalWrite(13, LOW);
  }
  
  permission = permission + 1UL;
}

void go(){
  permission = 0UL;
}







