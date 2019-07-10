// Serial menu functions
#include "serial.h"
#include "Chronodot.h"
#include "time.h"
#include "touchbuttons.h"
#include <EEPROM.h>

byte serialState = MENU;

char serialBuffer[SERIAL_BUFFER_SIZE]; 
byte ptr = 0;
boolean hasNewline = false;


void serialManager(unsigned long now)
{
  static unsigned long prev;
  
  // Proceed only if interval has passed
  if (now-prev < SERIAL_MENU_INTERVAL)
    return;
  prev = now;
  
  serialMenu();
}


// Process serial menu input
void serialMenu()
{

  static int hour;
  static int minute;
  
  hasNewline = false;
  while (Serial.available())
  {
    serialBuffer[ptr] = Serial.read();
    Serial.print(serialBuffer[ptr]); // Echo character
    
    if (serialBuffer[ptr] == '\n' ||
        serialBuffer[ptr] == '\r')
    {
      hasNewline = true;
      break;
    }
    
    ptr++;
    if (ptr == SERIAL_BUFFER_SIZE-1) // Buffer full, reset pointer (do not write over string terminator)
    {
      ptr = 0;
    }
  }
  if (!hasNewline) return;
  // At this point a newline must be in the buffer (hopefully after some number) 
  
  // Discard any extra bytes in the Serial buffer
  while (Serial.available()) Serial.read();
  
  // Get the written number (or 0 if empty/drunk/gibberish)
  int val = atoi(serialBuffer);

  // Clear our buffer
  memset(serialBuffer, 0, sizeof(serialBuffer));
  ptr = 0;
  
  
  // Proceed according to state and value entered
  
  switch (serialState)
  {
    case READ_INITIAL_MODE:
      if (val < 0 || TOTAL_MODES <= val)
      {
        Serial.println(F("Error: incorrect initial mode "));
      }
      else
      {
        EEPROM.write(EEPROM_INITIAL_MODE, val);
        Serial.print(F("Initial mode set to "));
        Serial.println(val);
      }
      serialState = MENU;
      printMenu();
      break;
    
    case READ_BOOT_MESSAGE:
      if (val < 0 || 1 < val)
      {
        Serial.println(F("Error: incorrect option"));
      }
      else
      {
        EEPROM.write(EEPROM_SHOW_BOOT, val);
        Serial.print(F("Boot message "));
        Serial.println(val ? F("enabled") : F("disabled"));
      }
      serialState = MENU;
      printMenu();
      break;
    case READ_HOUR:
  
      if (val < 0 || 23 < val)
      {
        Serial.println(F("Error: hour must be between 0 and 23"));
        serialState = MENU; // Don't ask for the minute
        printMenu();
        return;
      }
      else
      {
        hour = val;
      }
      Serial.println(F("Enter minute (0-59)"));
      serialState = READ_MINUTE;
      return;
      break;
   
    case READ_MINUTE:
      {
        if (val < 0 || 59 < val)
        {
          Serial.println(F("Error: minute must be between 0 and 59"));
          serialState = MENU; // Don't update the time
          printMenu();
          return;
        }
        else
        {
          minute = val;
        }
        
        // Adjust time with the new data
        DateTime newDateTime = DateTime(2015, 9, 12, hour, minute, 0, 32, 0.0);
        RTC.adjust(newDateTime); 
        Serial.print(F("Time adjusted to: "));
        printTime();
        Serial.println();
        serialState = MENU;
        printMenu();   
      }
      break;
      
    default: // Main menu
  
      if (val == 1) // Set initial mode
      {
        printModes();
        Serial.print(F("Enter initial mode >"));
        
        serialState = READ_INITIAL_MODE;
        return;
      }
      else if (val == 2) // Read initial mode
      {
        printModes();
        Serial.print(F("Initial mode: "));
        Serial.println(EEPROM.read(EEPROM_INITIAL_MODE));
        printMenu();
      }
      else if (val == 3) // Enable/disable boot message
      {
        Serial.print(F("Enter 1 to enable, 0 to disable >"));
        serialState = READ_BOOT_MESSAGE;
        return;
      }
      else if (val == 4) // Set time
      {
        Serial.print(F("Enter hour (0-23) >"));
        serialState = READ_HOUR;
        return;
      }
      else if (val == 5) // Get time
      {
        printTime();
        printMenu();
      }
      else
      {
        Serial.println(F("Please choose an option from 1 to 5"));
        printMenu();
      }
      break;
  }
}

// Print the menu on the serial port
void printMenu()
{
  Serial.println(F("Menu:"));
  Serial.println(F(" 1. Set initial mode"));
  Serial.println(F(" 2. Get initial mode"));
  Serial.println(F(" 3. Enable/disable boot message"));
  Serial.println(F(" 4. Set time"));
  Serial.println(F(" 5. Get time"));
  Serial.print('>');
}

void printModes()
{
  Serial.println(F("\
Wordclock     0\r\n\
Digital clock 1\r\n\
Temp humidity 2\r\n\
Analog clock  3\r\n\
Game of Life  4\r\n\
Simon         5\r\n\
Tetris        6\r\n\
Credits       7\r\n"));
}

// Prints the current time on the serial port
void printTime()
{
  DateTime now = RTC.now();  

  Serial.print(F("Current time: "));
  Serial.print(now.hour());
  Serial.print(':');
  if(now.minute() < 10) Serial.print('0');
  Serial.print(now.minute());
  Serial.print(':');
  if(now.second() < 10) Serial.print('0');
  Serial.println(now.second());
}

