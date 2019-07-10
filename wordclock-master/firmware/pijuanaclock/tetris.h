#ifndef TETRIS_H
#define TETRIS_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "stc.h"

#define BOARD_X (0)  // Where to put the board
#define BOARD_Y (0)

#define NXT_X (10)   // Where to draw the next tetromino
#define NXT_Y (2)

#define LEVEL_X (11) // Where to draw the level number (1 character)
#define LEVEL_Y (8)

#define ANALOG_SEED_PIN A6 // Unconnected pin

void tetrisManager(unsigned long now, boolean modeChanged);

void soundManager(unsigned long now, boolean modeChanged);

int platformInit(StcGame *gameInstance);

void platformEnd(StcGame *gameInstance);

long platformGetSystemTime();

void platformSeedRandom(long seed);

int platformRandom();

#endif

