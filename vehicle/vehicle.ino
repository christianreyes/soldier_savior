/*

Christian Reyes

Carnegie Mellon University
05-833 Gadgets, Sensors, and Activity Recognition in Human-Computer Interaction

5/1/2012

Project 3: Make a Cool Gadget

*/

#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"
#include "SerialCommand.h"

//-------------------------------------------------------------- define global variables --------------------------------------------

SerialCommand sCmd;

unsigned int Volts;
unsigned int LeftAmps;
unsigned int RightAmps;
unsigned long chargeTimer;
unsigned long leftoverload;
unsigned long rightoverload;
int highVolts;
int startVolts;
byte Charged=1;                                               // 0=Flat battery  1=Charged battery

unsigned long last_message = 10000UL;
boolean move = false;
int LeftPWM = 100;                                                  // PWM value for left  motor speed / brake
int RightPWM = 100;                                                 // PWM value for right motor speed / brake

//-------------------------------------------------------------- define servos ------------------------------------------------------

const int servo_pins[] = {2,4,7,8,9,10,12};
Servo servos[7];

void setup()
{
  //------------------------------------------------------------ Initialize Servos ----------------------------------------------------
  
  delay(2000);
  
  for(int i=0;i<7;i++){
    Servo s;
    s.attach(servo_pins[i]);
    s.writeMicroseconds(DServo);                          // set servo to default position
    servos[i] = s;
  }
  
  //------------------------------------------------------------ Initialize Communications ----------------------------------------------------
  
  sCmd.addCommand("SETSPEED", cmd_set_speed);
  
  Serial.begin(Brate);                                      // enable serial communication
}

void loop()
{
  voltage_check();   //-------- Check battery voltage and current draw of motors ------
  sCmd.readSerial();
  move_vehicle();    //-------- Move vehicle if charged
}

void cmd_set_speed(){
  move = true; 
  
  char *arg;
  
  // process incoming speeds
  
  arg = sCmd.next();
  if (arg != NULL) { LeftPWM = atoi(arg); Serial.println("Set left speed to : " + String(LeftPWM));}   // Converts a char string to an integer
  else { Serial.println("SETSPEED: No left PWM arg"); }

  arg = sCmd.next();
  if (arg != NULL) { RightPWM = atol(arg); Serial.println("Set right speed to : " + String(RightPWM));}   // Converts a char string to an integer
  else { Serial.println("SETSPEED: No left PWM arg"); }
}

void move_vehicle(){
  //----------------------------------------------------------- GOOD BATTERY speed controller opperates normally ----------------------
  // --------------------------------------------------------- Code to drive dual "H" bridges --------------------------------------
  last_message = last_message + 1UL;

  if ( move){         // Only power motors if battery voltage is good
    //Serial.println(permission);
    if ((millis()-leftoverload)>overloadtime){  // if left motor has not overloaded recently        
      l_motor(0, LeftPWM);                      // left motor forward
    }
    if ((millis()-rightoverload)>overloadtime){ // if right motor has not overloaded recently
      r_motor(0, RightPWM);                     // right motor forward
    } 
  } else{
    l_motor(0, 0); // stop the vehicle
    r_motor(0, 0);
  }
  
  if( last_message > 32000UL){
    last_message = 1500UL;
  }
}

void l_motor(int a_pwm, int b_pwm){
  analogWrite(LmotorA,a_pwm);  // send voltages to drive motors
  analogWrite(LmotorB,b_pwm);
}

void r_motor(int a_pwm, int b_pwm){
  analogWrite(RmotorA,a_pwm); // send voltages to drive motors
  analogWrite(RmotorB,b_pwm);
}

void voltage_check(){
  //-------- Check battery voltage and current draw of motors ------
  
  Volts=analogRead(Battery);                                  // read the battery voltage
  LeftAmps=analogRead(LmotorC);                               // read left motor current draw
  RightAmps=analogRead(RmotorC);                              // read right motor current draw
  //Serial.print(LeftAmps);Serial.print("    ");Serial.println(RightAmps); 
  
  if (LeftAmps>Leftmaxamps)                                   // is motor current draw exceeding safe limit
  {
    l_motor(0,0);                                             // turn off motors
    leftoverload=millis();                                    // record time of overload
    //Serial.println("left overload!");
  }

  if (RightAmps>Rightmaxamps)                                 // is motor current draw exceeding safe limit
  {
    r_motor(0,0);                                             // turn off motors
    rightoverload=millis();                                   // record time of overload
    //Serial.println("right overload!");
  }
  
  if ((Volts<lowvolt) && (Charged==1))                        // check condition of the battery
  {                                                           // change battery status from charged to flat

    //---------------------------------------------------------- FLAT BATTERY speed controller shuts down until battery is recharged ----
    //---------------------------------------------------------- This is a safety feature to prevent malfunction at low voltages!! ------

    Charged=0;                                                // battery is flat
    highVolts=Volts;                                          // record the voltage
    startVolts=Volts;
    chargeTimer=millis();                                     // record the time

    //digitalWrite (Charger,0);                                 // enable current regulator to charge battery
    Serial.println("NEEDS CHARGING!");
  }
  
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.print("Unrecognized command: ");
  Serial.println(command);
}






