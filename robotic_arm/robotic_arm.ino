#include <SoftwareSerial.h>

SoftwareSerial ss(2, 3);

const int killswitch = 7;

void setup()
{
  pinMode(killswitch, INPUT);
  Serial.begin(9600);
  ss.begin(4800);
  Serial.println("0.0,0.0");
}

// skeleton code
void loop()
{
  if(digitalRead(killswitch) == HIGH)
    Serial.println("!");
}
