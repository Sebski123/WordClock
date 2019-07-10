// Serial menu functions

#ifndef SERIAL_H
#define SERIAL_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define MENU                0
#define READ_INITIAL_MODE   1
#define READ_BOOT_MESSAGE   2
#define READ_HOUR           3 
#define READ_MINUTE         4

// EEPROM addresses
#define EEPROM_INITIAL_MODE 0 // Initial mode of the clock
#define EEPROM_SHOW_BOOT    1 // Wether to show the boot message
//#define EEPROM_TETRIS_HIGH_SCORE 2 :)

#define SERIAL_BUFFER_SIZE 5 // Space for 2 digits + '\r' + '\n' + '\0'
#define SERIAL_MENU_INTERVAL 100

void serialManager(unsigned long now);

// Process serial menu input
void serialMenu();

// Print the menu on the serial port
void printMenu();

// Print the menu Modes on serial port
void printModes();

// Prints the current time on the serial port
void printTime();


#endif

