/*
Program to read pot and set PWM out based on pot
 
  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - 
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

 this started from http://www.arduino.cc/en/Tutorial/AnalogInput
 AC Voltage dimmer with Zero cross detection
Author: Charith Fernanado http://www.inmojo.com
License: Creative Commons Attribution Share-Alike 3.0 License. 

Attach the Zero cross pin to Arduino External Interrupt pin
Select the correct Interrupt # from the below table:

 Pin    |  Interrrupt # | Arduino Platform
 ---------------------------------------
 d2      |  0            |  All
 d3      |  1            |  All

In the program pin d2 is chosen
*/
const byte interruptPin = 2; //interrupt on D2

#define BAUD 57600  // serial baud rate

// ************  Delay after stop  **************** 
// the time in usec to wait after zero-cross before switching off
// this eliminates the back-EMF from pump by waiting for current to drop before switching off
#define HALF_PERIOD 8333  //in usec 8333 for 60 hz, 10000 for 50 hz
#define DELAY_AFTER_STOP (HALF_PERIOD / 2)  //1/4 cycle = 4165

// the PWM pins are 3, 5, 6, 9, 10 and 11. 
//Frequency on pins 5 and 6 will be about 980Hz 
//pins 3, 9, 10 and 11 will be 490Hz

int InputPin = A0;    // select the input pin to use to set power level
int OutPin = 12;      // select the output pin
//const byte PWMoutPin = 3;      // select the pin for the slow output, about 490Hz pwm
int PowerLevel = 128;  // power level (0-128)  0 = full power, 128 = zero power
int IntCnt = 0;    // interrupt counter
int offtime = 0;
int ontime = 0;
int SaveIn=0;

void setup() {
  // declare OUTPUT and init serial port:
  pinMode(OutPin, OUTPUT);
  Serial.begin(BAUD);
  //setup interrupt
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), zero_crosss_int, RISING);
}

//Interrupt routine where all the work is done
void zero_crosss_int()  
// function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms  
  // Every zerocrossing thus: (50Hz)-> 10ms (1/2 Cycle) 
  // 10ms=10000us
  // (10000us - 10us) / 128 = 75 (Approx) 
  // 1 full 60hz = 1/60 = 16.67 ms, zero crossing 1/2 cycle 60Hz => 8.33ms
  // 8.3ms = 8300us (8300us-10us)/128 = 65

//wait for timeout time. if PowerLevel = 0 then full power. 
// 128*65=8320 microsecs, about 1/2 cycle
  
  delayMicroseconds(offtime);    // ssr off time after zero crossing
  digitalWrite(OutPin, HIGH);   // turn on ssr
  //set ssr on time to 1/2 cycle - offtime + the delay after stop 
  //See comments above for explanation of delay after stop
 // delayMicroseconds(4000); // ssr on time
 //  delayMicroseconds(8330 - offtime + DELAY_AFTER_STOP); // ssr on time
  delayMicroseconds(ontime); // ssr on time
  digitalWrite(OutPin, LOW);    // turn off ssr and exit interrupt
  IntCnt++;
  }

void loop() {
int aValue = 0;       // variable to store the value coming from the sensor
//  digitalWrite(PWMoutPin, LOW);    // turn off ssr
  // read the voltage at the input pin
  aValue = analogRead(InputPin);
  //calculate offtime and ontime when aValue changes
  if (SaveIn != aValue) {
    SaveIn = aValue;
    //PowerLevel = map(aValue, 0, 1023, 0, 128);
    PowerLevel = map(aValue, 0, 1023, 128, 0);
    offtime = (65*PowerLevel);    // For 60Hz (65*PowerLevel), For 50Hz (75*PowerLevel)  
    ontime = (8330 - offtime + DELAY_AFTER_STOP);  
  }
  //Set PowerLevel based on voltage at InputPin
  //Input pin is 0 to 1023 for 0-5V signal.  Map to range of 0 to 128 for PowerLevel
  //PowerLevel = 50;
//    PowerLevel = map(aValue, 0, 1023, 0, 128);
//  Serial.print("pot value: ");
 // offtime = (3000);    // For 60Hz (65*PowerLevel), For 50Hz (75*PowerLevel)    
  Serial.println(PowerLevel);
  delay(100);
}
