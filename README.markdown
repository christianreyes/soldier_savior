#Soldier Savior

A six wheeled autonomous vehicle which will be able to follow a soldier and respond to the soldier being injured. 

When the soldier's vitals indicate danger, the vehicle will use its robotic arm to autonomously deliver supplies. Additionally, the vehicle and arm will be able to be controlled manually via a RC transmitter.

##Serial Command Documentation

The following commands are supported over Serial to the Brain:

* `SETSPEED LeftPWMValue RightPWMValue`

For instance, to set the speed of the left wheels to 100 and the speed of the right wheels to 200: `Serial.println("SETSPEED 100 200");` or `Serial.println("SETSPEED " + String(LeftPWM) + " " + String(RightPWM));` (if you have the values in those variables).

##Libraries

* SerialCommand - Receive and process self-defined commands over Serial
* SoftwareSerial - Emulate Serial using I/O pins
* AltSoftSerial - Emulate Serial using I/O pins (used in conjunction with SoftwareSerial to provide and additional Serial)

##Final Project

05833: Gadgets, Sensors and Activity Recognition in Human-Computer Interaction

Carnegie Mellon University

* Technical lead and system architect: Christian Reyes
* Vehicle, Brain, and Communications: Christian Reyes
* Robotic Arm: Aderinsola Akintilo
* Human Vital Sensors: Almas Tuyakbayev