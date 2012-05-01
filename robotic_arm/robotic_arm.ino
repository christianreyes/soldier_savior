//------------------------------- Aderinsola Akintilo --------------------------------------------
//------------------------------- Sunday April 29th --------------------------------------------
//------------------------------- Gadgets Final Project --------------------------------------------

// Base Code for Servos Provided By:
// Sweep
// by BARRAGAN <http://barraganstudio.com> 

#include <Servo.h> 


//-------------------------- Define Global Variable And Servos -------------------------------------

Servo wrist;  // the wrist/forearm of the robot 
Servo claw;   // the hand of the robot 
Servo swivel; // the "elbow" of the robot

int pos = 0;    // variable to store the servo position 

//------------------------------ Initialize Servos ------------------------------------

void setup() 
{ 
  Serial.begin(9600); 
  claw.attach(9);  
  wrist.attach(7);  // attaches the servo on pin 9 to the servo object 
  swivel.attach(5); 
  pinMode(13, OUTPUT);     
  claw.writeMicroseconds(500);
  wrist.writeMicroseconds(2600);
  swivel.writeMicroseconds(2400); 
  delay(2000);

  // ---------- Explanation of .writeMicroseconds ----------
  // --- 500 or less is 0 degrees --- 
  // --- 2400 or more is 180 degrees --- 
  // --- 1270 in miliseconds, or 70 degrees, is MAX degrees for CLAW --- 

} 

void loop() 
{ 
  digitalWrite(13, HIGH);   // -- set the LED on for testing purposes --
  delay(100); 
  digitalWrite(13, LOW);

  move_entire_arm(); // -- one main method for entirety of robot arm functions --

}

void move_entire_arm()
{

  grip_claw(); // -- the claw grabs an object -- 
  //pick_up(); // -- the wrist bends backwards to pick it up -- 
  swivel_onto_car();
  pause(2); // -- the claw stays clamped by detaching itself -- 
  claw.attach(9); // -- reattach the claw so that it can drop the object. 
  release_claw(); // -- claw releases object -- 
  //reset_wrist(); // -- wrist goes back to original position -- 
  swivel_off_car();
  pause(2); // -- claw stays the way it is while wrist resets -- 
}

void swivel_off_car()
{
  for(pos = 0; pos < 180; pos += 1) // -- picking up --
    // -- for each degree -- 
  {
    swivel.write(pos); // - move the wrist - 
    delay(30); 
  }
}

void swivel_onto_car()
{
  for(pos = 180; pos>=1; pos-=1) // -- dropping off --
  {
    swivel.write(pos); 
    delay(30);  
  }
}


void move_wrist()
{
  for(pos = 0; pos < 180; pos += 1) // -- picking up --
    // -- for each degree -- 
  {
    wrist.write(pos); // - move the wrist - 
    delay(10); 
  }

  for(pos = 180; pos>=1; pos-=1) // -- dropping off --
  {
    wrist.write(pos); 
    delay(10);  
  }
  //  close_claw(); 
}


void pick_up()
{
  for(pos = 0; pos < 180; pos += 1) //picking up 
  {
    wrist.write(pos); 
    delay(30); 
  }
}

void reset_wrist()
{
  for(pos = 180; pos>=1; pos-=1) // dropping off
  {
    wrist.write(pos); 
    delay(30);  
  } 
}

void grip_claw()
{
  for(pos = 0; pos < 80; pos += 1) //  closing 
  {
    if (claw.read() == 75) // if the claw is closed
    {
      claw.detach();  // stop moving. 
    } 
    claw.write(pos); // else keep closing
    delay(30); 
  }
}

void release_claw()
{
  for(pos = 80; pos >= 1; pos -= 1) //  opening
  {
    claw.write(pos); 
    delay(20); 
  } 
}

void pause(int mili)
{
  int seconds = mili*1000; 
  delay(seconds); 
}



//-------------------------- Extra / Safety Code -------------------------------------

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





