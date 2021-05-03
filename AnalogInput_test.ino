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

 
 based on.
  http://www.arduino.cc/en/Tutorial/AnalogInput
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

#define BAUD 57600  // serial baud rate

#define HALF_PERIOD 8333  //in usec

// the time in usec to wait after zero-cross before switching off
// this eliminates the back-EMF by waiting for current to drop before switching off
#define DELAY_AFTER_STOP (HALF_PERIOD / 2)


// the PWM pins are 3, 5, 6, 9, 10 and 11. 
//Frequency on pins 5 and 6 will be about 980Hz 
//pins 3, 9, 10 and 11 will be 490Hz

int sensorPin = A0;    // select the input pin for the potentiometer
int PWMhighoutPin = 6;      // select the pin for the fast output, about 980Hz pwm
int PWMlowoutPin = 3;      // select the pin for the slow output, about 490Hz pwm
int PWMValue = 128;  // power level (0-128)  0 = full ON, 128 = OFF
int aValue = 0;       // variable to store the value coming from the sensor

//int dimming = 128;  // Dimming level (0-128)  0 = ON, 128 = OFF

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(PWMhighoutPin, OUTPUT);
  Serial.begin(BAUD);
}

void zero_crosss_int()  
// function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms  
  // Every zerocrossing thus: (50Hz)-> 10ms (1/2 Cycle) For 60Hz => 8.33ms
  // 10ms=10000us
  // (10000us - 10us) / 128 = 75 (Approx) For 60Hz =>65

//wait for timeout time, it time = 0 then full power 128*65=8320 microsecs
  
  int dimtime = (65*PWMValue);    // For 60Hz =>65     
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(PWMlowoutPin, HIGH);   // turn on ssr
  delayMicroseconds(8330 - dimtime + DELAY_AFTER_STOP);         // ssr on time
  digitalWrite(PWMlowoutPin, LOW);    // triac Off
}

void loop() {
  // read the value from the sensor:
  aValue = analogRead(sensorPin);
  PWMValue = map(aValue, 0, 1023, 0, 128);
//  Serial.print("pot value: ");
//  Serial.println(aValue);
 // Serial.print("sensor value: ");
 // Serial.println(PWMValue);
  // turn the ledPin on
 // digitalWrite(PWMhighoutPin,PWMValue );  //pin 6
 // digitalWrite(PWMlowoutPin,PWMValue );   //pin 3
  delay(100);
}
