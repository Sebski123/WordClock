// Brightness functions

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H
#define BOOT_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif



extern byte brightness; // Between 0 and 15


void brightnessManager(unsigned long now);

// Set the LED brightness automatically, depending on the ambient light reading
void setBrightnessAuto();

// Set the LED brightness (value between 0 and 15)
void setBrightness(int value);

#endif

