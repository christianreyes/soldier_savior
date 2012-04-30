#include <SoftwareSerial.h>

SoftwareSerial ss(NULL, 2); // RX, TX

boolean once = true;

void setup(){
  ss.begin(9600);
}

void loop(){
  if(once){
    delay(5000);
    ss.println("SETSPEED 100 100");
    delay(1500);
    ss.println("SETSPEED 100 220");
    delay(1500);
    ss.println("SETSPEED 220 100");
    delay(500);
    ss.println("SETSPEED 100 100");
    delay(500);
    ss.println("SETSPEED 0 0");
    once = false;
  }
}
