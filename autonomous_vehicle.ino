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

//-------------------------------------------------------------- define servos ------------------------------------------------------

const int servo_pins[] = {2,4,7,8,9,10,12};
Servo servos[7];

//-------------------------------------------------------------- define GPS ------------------------------------------------------

TinyGPS gps;
SoftwareSerial ss(gps_rx, gps_tx);

float mlat = 0.0;
float mlon = 0.0;
float tlat = 0.0;
float tlon = 0.0;

char inData[80];
byte index = 0;

void setup()
{
  //------------------------------------------------------------ Initialize Servos ----------------------------------------------------
  
  for(int i=0;i<7;i++){
    Servo s;
    s.attach(servo_pins[i]);
    s.writeMicroseconds(DServo);                          // set servo to default position
    servos[i] = s;
  }
  
  //------------------------------------------------------------ Initialize Communications ----------------------------------------------------
  
  Serial.begin(Brate);                                      // enable serial communications if Cmode=1
  Serial.println("I AM ALIVE!");
  
  ss.begin(4800);  // start GPS communication
}

void loop()
{
  voltage_check();   //-------- Check battery voltage and current draw of motors ------
  get_gps();         // update vehicle's GPS location
  processSerial();   // process incoming serial data
  move_vehicle();    //-------- Move vehicle if charged
}

void processSerial(){
  while(Serial.available() > 0) {
    char aChar = Serial.read();
    if(aChar == '\n') {
      // End of message detected. Time to parse
      if(String(inData).indexOf("!") != -1){
        Serial.println("SAFE!");  
      } else {
       
        char *p = inData; 
        char *str;
      
        int counter = 0; 
           
        while (str = strtok_r(p, ",", &p)){
          if(counter == 0){
            tlat = atof(str);
            counter = 1;
          } else {
            tlon = atof(str);
            counter = 0;
          }
        } 
      
        Serial.print("TLat: ");
        Serial.print(tlat, 10);
        Serial.print(" TLon: ");
        Serial.println(tlon, 10);
      }
      
      index = 0;
      inData[index] = NULL;
    } 
    else { // keep reading in the characters from incoming serial
      inData[index] = aChar;
      index++;
      inData[index] = '\0'; // Keep the string NULL terminated
    }
  }
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
    unsigned long age;
    gps.f_get_position(&mlat, &mlon, &age);
    
    //Serial.print(mylat, 10);
    //Serial.print(",");
    //Serial.println(mylon,10);
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





