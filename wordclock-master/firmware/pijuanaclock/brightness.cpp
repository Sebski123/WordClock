// Brightness functions
#include "brightness.h"
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "display.h"


#define PIN_LDR A3
#define AUTO_BRIGHTNESS_INTERVAL    750

byte brightness = 255; // Use an invalid value so that brightness is updated the first time


void brightnessManager(unsigned long now)
{
  static unsigned long prev;
  
  // Proceed only if interval has passed
  if (now-prev < AUTO_BRIGHTNESS_INTERVAL)
    return;
  prev = now;
  
  setBrightnessAuto(); 
}

// ADC values determined with a 500R-200M LDR to +5V and 20K resistor to ground (with capacitor in parallel to smooth transitions)
//TODO adjust them again since LED current increased to 20mA and it's now a bit too bright in dark places
#define LOWER_BOUND 50
#define UPPER_BOUND 400

#define HYSTERESIS 5 // in ADC counts, prevents oscillation

// Set the LED brightness automatically, depending on the ambient light reading
void setBrightnessAuto()
{
  static int8_t currentHysteresis = 0; // Do not use hysteresis the first time
  
  // Read ambient light sensor
  int lightReading = analogRead(PIN_LDR); // 0 to 1023
  Serial.print(F("lightReading="));Serial.println(lightReading);
  lightReading += currentHysteresis; // Add the current hysteresis value to the reading (this has the same effect as moving the thresholds)
  
  Serial.print(F("lightReading+hyster="));Serial.println(lightReading);
  lightReading = constrain(lightReading, LOWER_BOUND, UPPER_BOUND);
  Serial.print(F("constrained lightReading="));Serial.println(lightReading);
  
  // lightReading -> bright mapping
  int bright = map(lightReading, LOWER_BOUND, UPPER_BOUND, 0, 15);
  Serial.print(F("bright="));Serial.println(bright);
  
  if (bright != brightness)
  {
    if (bright < brightness) currentHysteresis = -HYSTERESIS; // Going down
    else                     currentHysteresis =  HYSTERESIS; // Going up
    
    brightness = bright;
    setBrightness(bright);
  }
}


