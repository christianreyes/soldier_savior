#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial ss(2, 3);

const int killswitch = 7;

void setup()
{
  pinMode(killswitch, INPUT);
  Serial.begin(9600);
  ss.begin(4800);
  Serial.println("0.0,0.0");
}

void loop()
{
  if(digitalRead(killswitch) == HIGH)
    Serial.println("!");
    
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

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
