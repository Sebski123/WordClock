// Display LED functions

#ifndef DISPLAY_H
#define DISPLAY_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "LedControl.h" // MAX7219 - LED matrix drivers - http://playground.arduino.cc/Main/LedControl

// Pins to led drivers
#define pinData  14  // A0 (used as digital pin)
#define pinLoad  15  // A1 (used as digital pin)
#define pinClock 16  // A2 (used as digital pin)


#define SIZE 16

// Buffer
//extern boolean     frame[SIZE][SIZE];  // Dimensions are [row][column]
//extern boolean prevframe[SIZE][SIZE];

// New space-saving buffers
extern byte     frame2[32];
extern byte prevFrame2[32];



#define noBoards 4

// Initializes the LED controllers
void initDisplay();

// Set the LED brightness (value between 0 and 15)
void setBrightness(int value);

// Turn all LEDs on or off
void setAllLeds(boolean state);

// Switch on or off a LED, applying the required transform
void setLed(byte row, byte col, boolean on);

// Frame functions

// Set a LED in the frame on or off
void setFrame(byte* frame2, byte row, byte col, boolean on);

// Get the state of an LED in the frame
boolean getFrame(byte* frame2, byte row, byte col);

// Show the frame on the display (only switch changed LEDs)
// Also copy the frame to the previous frame buffer
void updateDisplay();

// Clear the frame
void clearFrame();

// Add a word to the frame
void addWordToFrame(const byte theword[3]);

/*
//TODO remove if unnecessary
// Two smileys stacked vertically
const byte smileys[32] PROGMEM = 
{
  B00000000,B00000000,
  B00001100,B00110000,
  B00001100,B00110000,
  B00000000,B00000000,
  B00100000,B00000100,
  B00011100,B00111000,
  B00000111,B11100000,
  B00000000,B00000000,
  B00000000,B00000000,
  B00001100,B00110000,
  B00001100,B00110000,
  B00000000,B00000000,
  B00100000,B00000100,
  B00011100,B00111000,
  B00000111,B11100000,
  B00000000,B00000000
};
//NOTE a different sprite format can be decided

// Add an animation sprite to the frame with the specified offsets (>1s, not smooth)
void addSpriteToFrame(int offsetX, int offsetY, const byte animSprite[32]);

// Show an animation sprite with the specified offsets (bypassing frame) (~302ms max, depending on offsets)
void showAnimSprite(int offsetX, int offsetY, const byte animSprite[32]);

// Show a pretransformed animation sprite (~37ms constant)
void showAnimSpritePT(const byte animSprite[32]);

// Animation test
void animSmileys();

// Prova de velocitat
void testSpeed();
*/
#endif
