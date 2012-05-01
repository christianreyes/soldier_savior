
/*

Christian Reyes

Carnegie Mellon University
05-833 Gadgets, Sensors, and Activity Recognition in Human-Computer Interaction

5/1/2012

Project 3: Make a Cool Gadget

*/

#include <SoftwareSerial.h>
#include "AltSoftSerial.h"
#include "SerialCommand.h"
//#include "SoftwareSerialCommand.h"
#include "TinyGPS.h"

//-------------------------------------------------------------- define global variables --------------------------------------------

unsigned long permission = 100UL;
int LeftPWM = 0;                                                  // PWM value for left  motor speed / brake
int RightPWM = 0;                                                 // PWM value for right motor speed / brake

//-------------------------------------------------------------- define GPS ------------------------------------------------------

TinyGPS gps;
SoftwareSerial ss_gps(3, NULL);     // RX, TX
AltSoftSerial ss_vehicle; // uses pin 9 for RX by default
SerialCommand sCmd;

boolean target = false;
boolean self = false; 

float mlat = 0.0;
float mlon = 0.0;
float tlat = 0.0;
float tlon = 0.0;
float diff;

void setup() {
  pinMode(13, OUTPUT);
  
  ss_vehicle.begin(9600);  // start vehicle communication  
  ss_gps.begin(4800);      // start GPS communication
  Serial.begin(9600);  // start vehicle communication  

  sCmd.addCommand("TGPS_COORD", target_gps); // add serial command to process

  delay(5000);             // wait for vehicle systems to start
}

void loop() {
  //ss_vehicle.println("SETSPEED 158 200");
  delay(200);
  //ss_vehicle.println("SETSPEED 0 0"); 
  
  //delay(200);
  get_gps();               // update vehicle's GPS location
  calculate_movement();    // calculate how to get to target
  
  sCmd.readSerial();      // process incoming serial
  move_vehicle();           // command vehicle to move to target
  
  /*
  Serial.print(self);
  Serial.print(" ");
  Serial.print(target);
  Serial.print(" ");
  Serial.print(TinyGPS::distance_between( mlat, mlon, tlat, tlon));
  Serial.print(" ");
  Serial.println(TinyGPS::distance_between( mlat, mlon, tlat, tlon) > 20.0);
  */
  
}

void target_gps(){
  char *arg;
 
  arg = sCmd.next();
  if (arg != NULL){
    tlat= atof(arg);  // process the latitutde
  }
  
  arg = sCmd.next();
  if (arg != NULL) {
    target = true;   
    tlon = atof(arg); // process the latitude
  }
}

void go(){
  
}

void get_gps(){
  while (ss_gps.available()){ // gps data in the buffer
    char c = ss_gps.read();   // Serial.write(c ); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)){       // Did a new valid sentence come in?
      unsigned long age;
      gps.f_get_position(&mlat, &mlon, &age); // update position of self
      self = true;
    }
  }
}

void calculate_movement(){
  if(self && target && (TinyGPS::distance_between( mlat, mlon, tlat, tlon) > 15.0) ){
    /*
    float m_heading = gps.f_course();
    float t_heading = TinyGPS::course_to( mlat, mlon, tlat, tlon);
    
    diff = t_heading - m_heading;
    diff += (diff>180) ? -360 : (diff<-180) ? 360 : 0;    // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles
    
    if(diff > 15.0){
      LeftPWM = 200;  	
      RightPWM = 100;	  	
    } else if(diff < -15.0){  	
      LeftPWM = 100;  	
      RightPWM = 200;  	
    } else {	  	
      LeftPWM = 158;	
      RightPWM = 200;
    }
    */
    LeftPWM = 158;  // values to move straight
    RightPWM = 200;
  } else {
    LeftPWM = 0;    // stop the vehicle
    RightPWM = 0;
  }
}

void move_vehicle(){
  ss_vehicle.println("SETSPEED " + String(LeftPWM) + " " + String(RightPWM)); // send command to vehicle to move
}






