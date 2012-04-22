#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"
#include <SoftwareSerial.h>
#include "TinyGPS.h"

//-------------------------------------------------------------- define global variables --------------------------------------------

unsigned int Volts;
unsigned int LeftAmps;
unsigned int RightAmps;
unsigned long chargeTimer;
unsigned long leftoverload;
unsigned long rightoverload;
int highVolts;
int startVolts;
byte Charged=1;                                               // 0=Flat battery  1=Charged battery

boolean move = false;
int LeftPWM = 100;                                                  // PWM value for left  motor speed / brake
int RightPWM = 100;                                                 // PWM value for right motor speed / brake

int servo[7];

//-------------------------------------------------------------- define servos ------------------------------------------------------


Servo Servo0;                                                 // define servos
Servo Servo1;                                                 // define servos
Servo Servo2;                                                 // define servos
Servo Servo3;                                                 // define servos
Servo Servo4;                                                 // define servos
Servo Servo5;                                                 // define servos
Servo Servo6;                                                 // define servos

//-------------------------------------------------------------- define GPS ------------------------------------------------------

TinyGPS gps;
SoftwareSerial ss(gps_rx, gps_tx);

void setup()
{
  //------------------------------------------------------------ Initialize Servos ----------------------------------------------------

  Servo0.attach(S0);                                          // attach servo to I/O pin
  Servo1.attach(S1);                                          // attach servo to I/O pin
  Servo2.attach(S2);                                          // attach servo to I/O pin
  Servo3.attach(S3);                                          // attach servo to I/O pin
  Servo4.attach(S4);                                          // attach servo to I/O pin
  Servo5.attach(S5);                                          // attach servo to I/O pin
  Servo6.attach(S6);                                          // attach servo to I/O pin

  //------------------------------------------------------------ Set servos to default position ---------------------------------------

  Servo0.writeMicroseconds(DServo0);                          // set servo to default position
  Servo1.writeMicroseconds(DServo1);                          // set servo to default position
  Servo2.writeMicroseconds(DServo2);                          // set servo to default position
  Servo3.writeMicroseconds(DServo3);                          // set servo to default position
  Servo4.writeMicroseconds(DServo4);                          // set servo to default position
  Servo5.writeMicroseconds(DServo5);                          // set servo to default position
  Servo6.writeMicroseconds(DServo6);                          // set servo to default position

  //------------------------------------------------------------ Initialize I/O pins --------------------------------------------------

  Serial.begin(Brate);                                      // enable serial communications if Cmode=1
  Serial.println("I AM ALIVE!");
  
  ss.begin(4800);  // start GPS communication
}

void loop()
{
  voltage_check();   //-------- Check battery voltage and current draw of motors ------
  get_gps();
  move_vehicle();    //-------- Move vehicle if charged
}

void get_gps(){
  bool newData = false;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (ss.available()) // characters in the buffer
    {
      char c = ss.read();
       //Serial.write(c ); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print(flat, 10);
    Serial.print(",");
    Serial.println(flon,10);
  }
}

void move_vehicle(){
  //----------------------------------------------------------- GOOD BATTERY speed controller opperates normally ----------------------
  // --------------------------------------------------------- Code to drive dual "H" bridges --------------------------------------

  if (Charged==1 && move)                                           // Only power motors if battery voltage is good
  {
    if ((millis()-leftoverload)>overloadtime)  // if left motor has not overloaded recently        
    {                                          // left motor forward
        analogWrite(LmotorA,0);
        analogWrite(LmotorB,LeftPWM);
    }
    if ((millis()-rightoverload)>overloadtime) // if right motor has not overloaded recently
    {                                          // right motor forward
        analogWrite(RmotorA,0);
        analogWrite(RmotorB,RightPWM);
    } 
  }
  else                                                      // Battery is flat
  {
    analogWrite (LmotorA,0);                                // turn off motors
    analogWrite (LmotorB,0);                                // turn off motors
    analogWrite (RmotorA,0);                                // turn off motors
    analogWrite (RmotorB,0);                                // turn off motors
  }
}

void voltage_check(){
  //-------- Check battery voltage and current draw of motors ------
  
  Volts=analogRead(Battery);                                  // read the battery voltage
  LeftAmps=analogRead(LmotorC);                               // read left motor current draw
  RightAmps=analogRead(RmotorC);                              // read right motor current draw

  //Serial.print(LeftAmps);
  //Serial.print("    ");
  //Serial.println(RightAmps);
  //Serial.println("I AM STILL ALIVE!");
  
  if (LeftAmps>Leftmaxamps)                                   // is motor current draw exceeding safe limit
  {
    analogWrite (LmotorA,0);                                  // turn off motors
    analogWrite (LmotorB,0);                                  // turn off motors
    leftoverload=millis();                                    // record time of overload
    Serial.println("left overload!");
  }

  if (RightAmps>Rightmaxamps)                                 // is motor current draw exceeding safe limit
  {
    analogWrite (RmotorA,0);                                  // turn off motors
    analogWrite (RmotorB,0);                                  // turn off motors
    rightoverload=millis();                                   // record time of overload
    Serial.println("right overload!");
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





