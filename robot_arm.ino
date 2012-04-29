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
  // claw.writeMicroseconds(1400);
  //  wrist.writeMicroseconds(1800); 
} 

void loop() 
{ 
  digitalWrite(13, HIGH);   // set the LED on
  delay(100); 
  digitalWrite(13, LOW);
  // For the Wrist
  //move_wrist(); 
  claw_movements(); 

  // For the Claw


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

void move_wrist()
{
  for(pos = 0; pos < 200; pos += 1) //picking up 
  {
    wrist.write(pos); 
    delay(10); 
  }

  for(pos = 200; pos>=1; pos-=1) // dropping off
  {
    wrist.write(pos); 
    delay(10);  
  }
//  close_claw(); 
}

void claw_movements()
{
  for(pos = 0; pos < 50; pos += 1) //  closing 
  {
    if (claw.read() == 49) // if the claw is closed
    {
      claw.detach();  // stop moving. 
    } 
    claw.write(pos); // else keep closing
    delay(30); 
  }
  
  delay(6000); //keep grip while holding on to object. 
  claw.attach(3); // reattach the claw so that it can drop the object. 
  
  for(pos = 50; pos >= 1; pos -= 1) //  opening
  {
    claw.write(pos); 
    delay(20); 
  }
  delay(6000); //stay open while wrist resets. 
}


void turn_swivel()
{
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
}


