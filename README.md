# Espressure
Espresso machine, control vibe pump power output

This software runs on an Arduino to control the output of a vibe pump to allow changing the pump power for pressure profiling.
I used an Arduino Nano just to do this, makes it easier with dedicated processor.

Description of hardware is here
https://sites.google.com/view/coffee4randy/projects/pump-control 

Uses an optocoupler for zero crossing detection (ZCT) of AC power, and then generates an interrupt when it detects
Uses pin D2 for interrupt input

All relevant SW runs in the interrupt controller.  Normally you should keep interrupt handling short, reason to use a dedicated processor.

When ZCT interrupt comes in, pump power is off. Interrupt reads pump power level setting, power level (0-128)  0 = full ON, 128 = OFF
Multiplies power x 65 (for 60 Hz). Note that 126 * 65 = 8320 microsecs.  About 1/2 of 60 Hz waveform.

Leaves pump power off for dimtime = power x 65 microsecs.

Then turns on pump power for (8330 - dimtime + DELAY_AFTER_STOP). 

DELAY_AFTER_STOP = 8330/2, or 1/4 waveform.  It is there to stop inductive kickback from the pump when you turn it off.
