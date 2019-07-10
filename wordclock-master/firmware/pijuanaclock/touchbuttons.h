// Touch button functions

#ifndef TOUCH_H
#define TOUCH_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define BL_KEY_DOWN_BIT   1
#define BR_KEY_DOWN_BIT   2
#define TL_KEY_DOWN_BIT   4
#define TR_KEY_DOWN_BIT   8
#define BL_KEY_UP_BIT    16
#define BR_KEY_UP_BIT    32
#define TL_KEY_UP_BIT    64
#define TR_KEY_UP_BIT   128

#define BL_KEY_PRESSED_BIT 1
#define BR_KEY_PRESSED_BIT 2
#define TL_KEY_PRESSED_BIT 4
#define TR_KEY_PRESSED_BIT 8

extern byte keysUpDown;
extern byte keysPressed;


// Key event macros

// True as long as button is held down
#define BL_KEY_PRESSED (keysPressed & BL_KEY_PRESSED_BIT)
#define BR_KEY_PRESSED (keysPressed & BR_KEY_PRESSED_BIT)
#define TL_KEY_PRESSED (keysPressed & TL_KEY_PRESSED_BIT)
#define TR_KEY_PRESSED (keysPressed & TR_KEY_PRESSED_BIT)
#define ANY_KEY_PRESSED (keysPressed)

// True only once when button is pressed
#define BL_KEY_DOWN (keysUpDown & BL_KEY_DOWN_BIT)
#define BR_KEY_DOWN (keysUpDown & BR_KEY_DOWN_BIT) 
#define TL_KEY_DOWN (keysUpDown & TL_KEY_DOWN_BIT)
#define TR_KEY_DOWN (keysUpDown & TR_KEY_DOWN_BIT)
#define ANY_KEY_DOWN (keysUpDown & B00001111)
           
// True only once when button is released
#define BL_KEY_UP (keysUpDown & BL_KEY_UP_BIT) 
#define BR_KEY_UP (keysUpDown & BR_KEY_UP_BIT)
#define TL_KEY_UP (keysUpDown & TL_KEY_UP_BIT)
#define TR_KEY_UP (keysUpDown & TR_KEY_UP_BIT)
#define ANY_KEY_UP (keysUpDown & B11110000)



// Pins to capacitive touch chips (touch and presence for each of the Azoteq IQS127D chips in the four corners)
#define pinTRB 9   // D9  Touch Right Bottom
#define pinTRT 3   // D3  Touch Right Top
#define pinTLT 4   // D4  Touch Left Top
#define pinTLB 10  // D10 Touch Left Bottom
// The presence pins are connected in hardware, but not used in this firmware.
// Leaving the pin numbers in as comments for future reference.
//#define pinPRB 5 // Presence Right Bottom
//#define pinPRT 8 // Presence Right Top
//#define pinPLT 7 // Presence Left Bottom
//#define pinPLB 6 // Presence Left Top



//TODO relocate?
// Initially the default clock display mode is word clock
#define INITIAL_MODE       0

#define MODE_WORD_CLOCK    0
#define MODE_DIGITAL_CLOCK 1
#define MODE_TEMP_HUMIDITY 2
#define MODE_ANALOG_CLOCK  3
#define MODE_GAME_OF_LIFE  4
#define MODE_SIMON         5
#define MODE_TETRIS        6
#define MODE_CREDITS       7

#define TOTAL_MODES        8
extern byte displayMode;

// Go to the next mode
void goToNextMode();

// Go to the previous mode
void goToPrevMode();

// Go directly to a specific mode
void goToMode(int8_t mode);

// Clear screen when going from a mode to another, using both the frame buffer and direct LED access
void cleanUp();

// Initialize touch sensor pins
void initTouch();

// Read all four buttons and update key states
void readTouch();

#endif

