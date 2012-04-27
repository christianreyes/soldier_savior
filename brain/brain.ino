#include <SoftwareSerial.h>
#include "SerialCommand.h"
#include "SoftwareSerialCommand.h"
#include "TinyGPS.h"

//-------------------------------------------------------------- define global variables --------------------------------------------
unsigned long permission = 10000UL;
boolean move = false;
int LeftPWM = 100;                                                  // PWM value for left  motor speed / brake
int RightPWM = 100;                                                 // PWM value for right motor speed / brake

//-------------------------------------------------------------- define GPS ------------------------------------------------------

TinyGPS gps;
SoftwareSerial ss_gps(2, 3);
SoftwareSerial ss_vehicle(4, 5);
SerialCommand sCmd;

float mlat = 0.0;
float mlon = 0.0;
float tlat = 0.0;
float tlon = 0.0;

void setup() {
  Serial.begin(9600);                                      // enable serial communications if Cmode=1
  ss_gps.begin(4800);  // start GPS communication  
  ss_vehicle.begin(9600);
}


void loop() {
  get_gps();         // update vehicle's GPS location
  calculate_movement();
  ss_vehicle.println("SETSPEED " + String(LeftPWM) + " " + String(RightPWM));
}

void calculate_movement(){
  float m_heading = gps.f_course();
  float t_heading = TinyGPS::course_to( mlat, mlon, tlat, tlon);
  float diff = t_heading - m_heading;
  
  diff += (diff>180) ? -360 : (diff<-180) ? 360 : 0;    // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles

  LeftPWM = (diff > 10) ? 200 : (diff < -10) ? 150 : 220;
  RightPWM = (diff > 10) ? 150 : (diff < -10) ? 200 : 220;
}


void get_gps(){
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 500UL;)
  {
    while (ss_gps.available()) // characters in the buffer
    {
      char c = ss_gps.read(); //Serial.write(c ); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)){ // Did a new valid sentence come in?
        unsigned long age;
        gps.f_get_position(&mlat, &mlon, &age);
      }
    }
  }
}





