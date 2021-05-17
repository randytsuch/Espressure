// user.h
// This file contains user definable compiler directives

// *************************************************************************************
// NOTE TO USERS: the following parameters should be
// be reviewed to suit your preferences and hardware setup.  
// First, load and edit this sketch in the Arduino IDE.
// Next compile the sketch and upload it to the Arduino.

// ------------------ optionally, use I2C port expander for LCD interface
//#define LCDAPTER // comment this line out to use the standard parallel LCD 4-bit interface
//#define CELSIUS // if defined, output is in Celsius units; otherwise Fahrenheit

#define BAUD 57600  // serial baud rate
//#define BAUD 115200
#define BT_FILTER 10 // filtering level (percent) for displayed BT
#define ET_FILTER 10 // filtering level (percent) for displayed ET

// use RISE_FILTER to adjust the sensitivity of the RoR calculation
// higher values will give a smoother RoR trace, but will also create more
// lag in the RoR value.  A good starting point is 80%, but for air poppers
// or other roasters where BT might be jumpy, then a higher value of RISE_FILTER
// will be needed.  Theoretical max. is 99%, but watch out for the lag when
// you get above 95%.
#define RISE_FILTER 85 // heavy filtering on non-displayed BT for RoR calculations
#define ROR_FILTER 80 // post-filtering on the computed RoR values for smooth display

// ambient sensor should be stable, so quick variations are probably noise -- filter heavily
#define AMB_FILTER 70 // 70% filtering on ambient sensor readings

// *************************************************************************************

/*
1. param.txt definition:
   1        2         3        4         5          6        7           8            9            10       11         12        13       14
Max pwr, min pwr, max flow, min flow, max pres, min pres, pulse pwr, pulse flow, max shot time, spare 10, spare 11, spare 12, spare 13, spare 14, 

Min and max pump power sets the limits for the analog output (via DAC) for the pump. Note the arduino
reads this voltage, and sets pump power based on the voltage. Pump power is scaled from 0-100
Min and max flow rate. When shot profile uses flow rates, these are the limits for the flow rate
Min and max pressure. When shot profile uses pressure, these are the limits for the pressure
pulse power, pulse flow. When the shot profile pulses the power or flow during the main part of the shot,
these are the lower limits used for pulse mode.
Max shot time. If a shot exceeds this time, pump power will be set to min power.

 * 2. shot.txt definition
Every line in the txt file is a different profile, except comment lines
If a line starts with // it is treated as a comment and not used by the program
Program uses a very simple, non forgiving algorithm to read in the lines
Shot Mode (1) Time preinfusion, power preinfusion, rampup time, shot power, shot time, rampdown
time. Prefusion with power control.
Shot Mode (2) Time preinfusion, power preinfusion, rampup time, shot flow, shot time, rampdown time.
Prefusion with flow control.
Shot Mode (3) Preinfusion pressure, power preinfusion, rampup time, shot power, shot time, rampdown
time
Pressure controlled prefusion with power control
Shot Mode (4) Preinfusion pressure, power preinfusion, rampup time, shot flow, shot time, rampdown
time
Pressure controlled prefusion with flow control
Shot Mode (5) - (10) Reserved, TBD
Shot Mode (11) = Mode 1 with pulsing power
Shot Mode (12) = Mode 2 with pulsing flow
Shot Mode (13) = Mode 3 with pulsing power
Shot Mode (14) = Mode 4 with pulsing flow

1        2         3         4        5         6          7          8         9         10        11        12       13       14       15   , name
index,time pre, pwr pre, pres pre, flow pre, Rup time, shot time, shot pwr, shot flow, shot pres, Rdn time, spare12, spare13, spare14, spare15, prof nane
int,  int secs,   int   ,int,      int,       int secs, int secs,    int .     ,int .     ,int    int secs,  int,    int,       int,     int,   string
3         6        10,     14,      18,         21,      24,         28,       32,        36,       39,      43,      47,       51,       55,    80 

 * 
 */
 
#define PARAM_SIZE 15        //number of parameters in parameter.txt file 
#define SHOT_ROW_SIZE 50     //max number of rows in the shot.txt file = max number of profiles 
#define SHOT_PARAM_SIZE 80   //number of chars per profile (column size) 
