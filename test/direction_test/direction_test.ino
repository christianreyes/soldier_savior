#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial ss(2, 3);

const int killswitch = 7;

float mlat = 0.0;
float mlon = 0.0;
float tlat = 40.44313300688997;
float tlon = -79.94024634361267;

int LeftPWM = 100;
int RightPWM = 100;

void setup()
{
  pinMode(killswitch, INPUT);
  Serial.begin(9600);
  ss.begin(4800);
  Serial.println("0.0,0.0");
}

void loop() {
  get_gps();         // update vehicle's GPS location
  calculate_movement();
}

void calculate_movement(){
  float m_heading = gps.f_course();
  float t_heading = TinyGPS::course_to( mlat, mlon, tlat, tlon);
  float diff = t_heading - m_heading;
  
  diff += (diff>180) ? -360 : (diff<-180) ? 360 : 0;    // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles

  LeftPWM = (diff > 10) ? 200 : (diff < -10) ? 150 : 220;
  RightPWM = (diff > 10) ? 150 : (diff < -10) ? 200 : 220;
  
  Serial.print(mlat,20);
  Serial.print(",");
  Serial.print(mlon,20);
  Serial.print(",");
  Serial.print(m_heading);
  Serial.print(",");
  Serial.print(TinyGPS::cardinal(m_heading));
  Serial.print(",");
  Serial.print(t_heading);
  Serial.print(",");
  Serial.print(TinyGPS::cardinal(t_heading));
  Serial.print(",");
  Serial.print(diff);
  Serial.print(",");
  Serial.print(TinyGPS::distance_between(mlat,mlon,tlat,tlon));
  Serial.print(",");
  Serial.print(LeftPWM);
  Serial.print(",");
  Serial.print(RightPWM);
  Serial.print(",");
  Serial.print(gps.f_speed_mph());
  Serial.print(",");
  Serial.print(gps.f_speed_mps());
  Serial.print(",");
  Serial.print(gps.satellites());
  Serial.print(",");
  unsigned long date;
  unsigned long time;
  unsigned long age;
  gps.get_datetime(&date, &time, &age);
  Serial.print(date);
  Serial.print(",");
  Serial.print(time);
  Serial.print(",");
  Serial.println(age);
}


void get_gps(){
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 500UL;)
  {
    while (ss.available()) // characters in the buffer
    {
      char c = ss.read(); //Serial.write(c ); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)){ // Did a new valid sentence come in?
        unsigned long age;
        gps.f_get_position(&mlat, &mlon, &age);
      }
    }
  }
}



