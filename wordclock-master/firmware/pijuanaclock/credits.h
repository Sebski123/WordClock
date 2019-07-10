#ifndef CREDITS_H
#define CREDITS_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define DELAY_AFTER_LETTER  500
#define DELAY_AFTER_NAME   1000


// Boot message words
const byte   w_boot_josep[3] PROGMEM = { 3,  2, 5 };
const byte       w_boot_i[3] PROGMEM = { 3,  8, 1 };
const byte     w_boot_ana[3] PROGMEM = { 3, 10, 3 };
const byte    w_boot_dels[3] PROGMEM = { 13, 1, 4 };
const byte      w_boot_de[3] PROGMEM = { 13, 6, 2 };
const byte      w_boot_la[3] PROGMEM = { 13, 9, 2 };
const byte     w_boot_uni[3] PROGMEM = { 13,12, 3 };
const byte      w_boot_12[3] PROGMEM = { 14, 4, 2 };
const byte     w_boot_set[3] PROGMEM = { 14, 7, 3 };
const byte    w_boot_2015[3] PROGMEM = { 14,11, 4 };

// Show the boot message
void showBootMessage();


#define NUM_NAMES 10

const byte numLettersByName[] PROGMEM =
{
  7, //Credits
  5, //Aleix
  7, //Antonio
  5, //Cesar
  5, //Enric
  6, //Isabel
  5, //Jordi
  5, //Laura
  5, //Oriol
  5  //Sergi 
};
const byte letters[] PROGMEM =
{
  // Credits Title
   1, 9,
   1,14,
   1,15,
   2, 4,
   2,11,
   2,14,
   2,15,
  
  // Aleix
   0,11,
   0,12,
   0,14,
   1, 4,
   2, 9,

  //Antonio
   4, 4,
   4, 5,
   5, 5,
   6, 5,
   6,13,
   6,14,
   6,15,
  
  //Cesar
   6, 1,
   7, 2,
   7, 3,
   7,12,
   7,14,

  //Enric
  10, 0,
  10, 1,
  12,10,
  12,11,
  12,12,
  
  //Isabel
   1, 4,
   1, 5,
   1,12,
   4, 3,
   4, 8,
   4,12,
  
  //Jordi
  13, 0,
  13, 5,
  13, 8,
  13,11,
  13,15,

  //Laura
   4, 6,
   4, 7,
   6, 7,
   7, 6,
   7,12,
  
  //Oriol
   2, 5,
   3, 7,
   3, 8,
   3, 9,
   4, 9,
   
  //Sergi
  14, 6,
  14,10,
  14,15,
  15, 6,
  15,12
};

void creditsManager(unsigned long now, boolean modeChanged);

void setupCreditsNames();

void iterateCreditsNames();

#endif


