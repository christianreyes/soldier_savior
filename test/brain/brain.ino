#include <SoftwareSerial.h>
#include "SerialCommand.h"
//#include "SoftwareSerialCommand.h"
#include "TinyGPS.h"

//-------------------------------------------------------------- define global variables --------------------------------------------

unsigned long permission = 100UL;
int LeftPWM = 0;                                                  // PWM value for left  motor speed / brake
int RightPWM = 0;                                                 // PWM value for right motor speed / brake

//-------------------------------------------------------------- define GPS ------------------------------------------------------

TinyGPS gps;
//SoftwareSerial ss_gps(3, NULL);     // RX, TX
SoftwareSerial ss_vehicle(NULL, 2); // RX, TX
SerialCommand sCmd;

boolean target = true;
boolean self = false; 

float mlat = 0.0;
float mlon = 0.0;
float tlat = 40.44268902361984;
float tlon =  -79.93807911872864;
float diff;

void setup() {
  pinMode(13, OUTPUT);
  delay(2000);             // wait for vehicle systems to start
  //Serial.begin(9600);      // enable wireless serial communications

  Serial.begin(4800);      // start GPS communication
  ss_vehicle.begin(9600);  // start vehicle communication  

  //Serial.println("ONLINE BRAIN");
}

void loop() {
  delay(250);
  get_gps();               // update vehicle's GPS location
  calculate_movement();    // calculate how to get to target
  move_vehicle();          // command vehicle to move to target
}

void get_gps(){
  while (Serial.available()){ // gps data in the buffer
    char c = Serial.read();   // Serial.write(c ); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)){       // Did a new valid sentence come in?
      unsigned long age;
      gps.f_get_position(&mlat, &mlon, &age); // update position of self
      self = true;
      //digitalWrite(13, HIGH);
      /*
      Serial.print(mlat);
      Serial.print(" ");
      Serial.print(mlon);
      Serial.print(" ");
      Serial.print(LeftPWM);
      Serial.print(" ");
      Serial.println(RightPWM);
      */
    }
  }
}

void calculate_movement(){
  if(self && target && TinyGPS::distance_between( mlat, mlon, tlat, tlon) > 3.0 ){
    float m_heading = gps.f_course();
    float t_heading = TinyGPS::course_to( mlat, mlon, tlat, tlon);
    diff = t_heading - m_heading;
    
    diff += (diff>180) ? -360 : (diff<-180) ? 360 : 0;    // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles
  
    LeftPWM = (diff > 10) ? 200 : (diff < -10) ? 80 : 230;
    RightPWM = (diff > 10) ? 80 : (diff < -10) ? 200 : 230; 
  } else {
    LeftPWM = 0;
    RightPWM = 0;
  }
}

void move_vehicle(){
  ss_vehicle.print("SETSPEED ");
  ss_vehicle.print(LeftPWM);
  ss_vehicle.print(" ");
  ss_vehicle.println(RightPWM);
}






