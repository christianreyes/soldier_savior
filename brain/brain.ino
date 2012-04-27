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
SerialCommand sCmd;

float mlat = 0.0;
float mlon = 0.0;
float tlat = 0.0;
float tlon = 0.0;

void setup() {
  Serial.begin(9600);                                      // enable serial communications if Cmode=1
  ss.begin(4800);  // start GPS communication
  
  sCmd.addCommand("SETSPEED", cmd_set_speed);
}

void cmd_set_speed(){
  
}

void loop() {
  processSerial();   // process incoming serial data
  get_gps();         // update vehicle's GPS location
  calculate_movement();
  move_vehicle();    //-------- Move vehicle if charged
}

void calculate_movement(){
  float m_heading = gps.f_course();
  float t_heading = TinyGPS::course_to( mlat, mlon, tlat, tlon);
  float diff = t_heading - m_heading;
  
  diff += (diff>180) ? -360 : (diff<-180) ? 360 : 0;    // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles

  LeftPWM = (diff > 10) ? 200 : (diff < -10) ? 150 : 220;
  RightPWM = (diff > 10) ? 150 : (diff < -10) ? 200 : 220;
/*  
  if(diff > 10){
    LeftPWM = 200;
    RightPWM = 100;
  } else if(diff < -10){
    LeftPWM = 100;
    RightPWM = 200;
  } else {
    LeftPWM = 200;
    RightPWM = 200;
  }
  */
  
  
  //Serial.println(int(diff));
}

void processSerial(){
  while(Serial.available() > 0) {
    char aChar = Serial.read();
    if(aChar == '\n') {
      // End of message detected. Time to parse

        permission = 1UL;
      }
      
  }
}

void get_gps(){
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 500UL;)
  {
    while (ss.available()) // characters in the buffer
    {
      char c = ss.read(); //Serial.write(c ); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)){ // Did a new valid sentence come in?
        unsigned long age;
        gps.f_get_position(&mlat, &mlon, &age);
      }
    }
  }
}

void move_vehicle(){
  permission = permission + 1UL;

  if ( permission < 5UL ){         
    //l_motor(0, LeftPWM);                      // left motor forward
    //r_motor(0, RightPWM);                     // right motor forward
  } else{
    //l_motor(0, 0);
    //r_motor(0, 0);
  }
  
  if( permission > 32000UL){
    permission = 1500UL;
  }
}







