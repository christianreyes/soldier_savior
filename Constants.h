//=================================== MODE OF COMMUNICATIONS ========================================================================

#define Brate             9600     // Baud rate for serial communications

//=================================== BATTERY CHARGER SETTINGS ======================================================================

#define batvolt            487     // This is the nominal battery voltage reading. Peak charge can only occur above this voltage.
#define lowvolt            410     // This is the voltage at which the speed controller goes into recharge mode.
#define chargetimeout   300000     // If the battery voltage does not change in this number of milliseconds then stop charging.


//=================================== H BRIDGE SETTINGS =============================================================================

#define Leftmaxamps        800     // set overload current for left motor 
#define Rightmaxamps       800     // set overload current for right motor 
#define overloadtime       100     // time in mS before motor is re-enabled after overload occurs


//=================================== SERVO SETTINGS ================================================================================

#define DServo            1500     // default position for servo0 on "power up" - 1500uS is center position on most servos



