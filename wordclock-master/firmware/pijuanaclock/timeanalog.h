#ifndef TIMEANALOG_H
#define TIMEANALOG_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define MINUTE_HAND_LENGTH 5
#define HOUR_HAND_LENGTH 3
const float pi = 3.141592;

void line(int x0, int y0, int x1, int y1, boolean state);

void drawTime(int h,int minutes);

// Show the current time on the display using an analog clock
void showTimeAnalog();

#endif

