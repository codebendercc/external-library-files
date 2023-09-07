SimpleSoftwareServo 0.1 by Tyler Bletsch (Tyler.Bletsch@gmail.com)

Based on SoftwareServo (http://playground.arduino.cc/ComponentLib/servo)
With pieces from Servo8Bit by CunningTurtle (http://www.cunningturtle.com/servo8bit-library-version-0-6-released/)


Requirements:
 - Microcontroller running at >= 8 MHz
 - The user must call refresh() each time a pulse needs to be generated


Installation:
 - Extract to the 'libraries' directory under your sketchbook directory.

 
This is a replacement for SoftwareServo, which was great for ATtiny controllers,
except for two problems:
 1. It didn't include "Arduino.h", so it didn't compile (?)
 2. It played pretty loose with the inter-pulse time, waiting until AFTER 
    20ms to pulse again, so if you had a delay of 15ms before the 
    refresh, you'd wait 15+15=30ms plus computation time between pulses, 
    which is fine for servos, but might upset the motor controller I'm using.

So I took it and rewrote the refresh method and a few other things to 
standardize the pulse-to-pulse time rather than the inter-pulse time.
Now the output on my oscilloscope is gorgeous.

It's nearly a drop-in replacement for SoftwareServo, but be aware:
It will generate a pulse EVERY time refresh is called, unlike SoftwareServo,
which would ignore calls that were too close together. This may change
behavior in some cases.  I prefer this behavior, because it avoids
uneven pulse timing.

