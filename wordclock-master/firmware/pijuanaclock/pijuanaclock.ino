// Pijuana Wordclock Project firmware
// ==================
// Based on the Wordclock by Wouter Devinck (August 2014)
// Translation and extension of that project, adding Catalan hours format and many new modes.
// July 2015 - September 2015
 
// Aleix Mercader, Antonio Camacho, Cesar August Pomerol, Enric Masdeu,
// Isabel Martínez, Jordi Aranda, Laura Mampel, Oriol Bernadó, Sergi Vaqué

// Dependencies:
//  * Arduino libraries                - http://arduino.cc/
//  * Chronodot library (for DS3231)   - https://github.com/Stephanie-Maks/Arduino-Chronodot
//  * LedControl library (for MAX7219) - http://playground.arduino.cc/Main/LedControl
//  * Temp. sensor library (for DHT22) - https://github.com/adafruit/DHT-sensor-library
//  * Simple Tetris Clone library      - https://github.com/ex/blocks

/* Hardware block diagram:

              +-----------------+
              | Real time clock |
              | Maxim DS3231    |
              +--------+--------+
                       |I2C
         +-------------+-------------+
+-----+  |                           |   +------------------+
|DHT22+--|                           +---+ 8x8 LED matrix 1 |
+-----+  |                           |   | Maxim MAX7219    |
         |                           |   +---------+--------+
+---+    |      Microcontroller      |             |
|LDR+----+      Atmel ATMEGA328      |   +---------+--------+
+---+    |      (with Arduino        |   | 8x8 LED matrix 2 |
         |       bootloader)         |   | Maxim MAX7219    |
+------+ |                           |   +---------+--------+
|Buzzer+-+                           |             |
+------+ |                           |   +---------+--------+
         +-++----++---------++----++-+   | 8x8 LED matrix 3 |
           ||    ||         ||    ||     | Maxim MAX7219    |
    +------++-+  ||  +------++-+  ||     +---------+--------+
    | Azoteq  |  ||  | Azoteq  |  ||               |
    | IQS127D |  ||  | IQS127D |  ||     +---------+--------+
    +---------+  ||  +---------+  ||     | 8x8 LED matrix 4 |
                 ||               ||     | Maxim MAX7219    |
          +------++-+      +------++-+   +------------------+
          | Azoteq  |      | Azoteq  |
          | IQS127D |      | IQS127D |
          +---------+      +---------+

(created using http://asciiflow.com/) */


// Includes
#include <EEPROM.h>
#include <Wire.h>


#include "brightness.h"
#include "character.h"
#include "credits.h"
#include "display.h"
#include "life.h"
#include "serial.h"
#include "simon.h"
#include "temphum.h"
#include "tetris.h"
#include "time.h"
#include "timeanalog.h"
#include "touchbuttons.h"

void setup() {

  // Debug info
  Serial.begin(9600);
  Serial.println(F("Wordclock is booting"));
  
  // Initiate the LED drivers
  // Brightness is set to max by default
  Serial.println(F("Init LED drivers"));
  initDisplay();

  // Initiate the Real Time Clock
  Serial.println(F("Init RTC"));
  initRTC();

  // Initiate the temperature and humidity sensor
  Serial.println(F("Init temp sensor"));
  initTempHum();

  // Initiate the capacitive touch inputs
  Serial.println(F("Init touch sensor"));
  initTouch();

  // Read settings from EEPROM
  Serial.println(F("Read settings"));
  byte initial = EEPROM.read(EEPROM_INITIAL_MODE);
  boolean showBoot = EEPROM.read(EEPROM_SHOW_BOOT);

  setBrightness(15);
  
  // Boot message
  if (showBoot)
    showBootMessage();
  
  Serial.println(F("Done. Hello!"));

  // Show serial menu
  printMenu();

  // Go to initial mode
  goToMode(initial);
}


void loop() {

  // Camacho's state machine
  //getInput();
  readTouch();
  stateManager();
  //setOutput();
}



void stateManager()
{
  static byte prevDisplayMode = -1;
  
  unsigned long now = millis();

  boolean modeChanged = prevDisplayMode != displayMode;
  prevDisplayMode = displayMode;

  // Run state-dependent code
  switch (displayMode)
  {
    case MODE_WORD_CLOCK:
    case MODE_DIGITAL_CLOCK:
    case MODE_ANALOG_CLOCK:

      timeManager(now, modeChanged);
      break;

    case MODE_TEMP_HUMIDITY:

      tempHumManager(now, modeChanged);
      break;

    case MODE_GAME_OF_LIFE:
    
      gameOfLifeManager(now, modeChanged);
      break;

    case MODE_SIMON:
    
      simonManager(now, modeChanged);
      break;

    case MODE_TETRIS:
      
      tetrisManager(now, modeChanged);
      break;
      
    case MODE_CREDITS:

      creditsManager(now, modeChanged);
      break;
      
    default:
      break;
  }

  // State-independent code
  brightnessManager(now);
  serialManager(now);
}
