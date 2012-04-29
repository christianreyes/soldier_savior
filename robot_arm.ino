// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 

Servo wrist;  // create servo object to control a servo 
Servo claw;// a maximum of eight servo objects can be created 
Servo swivel; 

int pos = 0;    // variable to store the servo position 
int posi = 0;

void setup() 
{ 
  Serial.begin(9600);       
  wrist.attach(5);  // attaches the servo on pin 9 to the servo object 
  claw.attach(3);
  swivel.attach(2); 
   pinMode(13, OUTPUT);  
//  claw.writeMicroseconds(1400);
//  wrist.writeMicroseconds(1800); 
} 

void loop() 
{ 
 digitalWrite(13, HIGH);   // set the LED on

  // For the Wrist
  for(pos = 0; pos < 200; pos += 1) //picking up 
  {
    wrist.write(pos); 
    delay(15); 
  }
  for(pos = 200; pos>=1; pos-=1) // dropping off
  {
    wrist.write(pos); 
    delay(15);  
  }
  
    for(pos = 0; pos < 200; pos += 1) //picking up 
  {
    swivel.write(pos); 
    delay(35); 
  }
  for(pos = 200; pos>=1; pos-=1) // dropping off
  {
    swivel.write(pos); 
    delay(35);  
  }
  
  
  // For the Claw
   for(pos = 0; pos < 160; pos += 1) //closing up 
  {
    
    //Serial.println("the wrist's degrees: " + wrist.read()); 
     
    claw.write(pos); 
    delay(50); 
  }

digitalWrite(13, LOW);
  //   for(pos = 160; pos>=1; pos-=1)   //opening  // goes from 180 degrees to 0 degrees 
  //  {                             
  ////    wrist.write(pos); 
  ////    delay(pos/5); 
  //  wrist.write(pos/5); 
  //  delay(10); 
  //  }

  //  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  //  {                                  // in steps of 1 degree 
  //    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  //    delay(5);    
  //    two.write(pos);
  //        // waits 15ms for the servo to reach the position 
  //  } 
  //  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  //  {                                
  //    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  //    delay(5); 
  // two.write(pos);
  //        // waits 15ms for the servo to reach the position 
  //  } 
}

