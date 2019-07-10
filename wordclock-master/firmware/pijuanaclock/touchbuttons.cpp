// Touch button functions
#include "display.h"
#include "touchbuttons.h"

byte displayMode = INITIAL_MODE;

void goToNextMode()
{
  if (displayMode == TOTAL_MODES-1)
    displayMode = 0; // Cycle through modes
  else
    displayMode++;
  
  cleanUp();
}

void goToPrevMode()
{
  if (displayMode == 0)
    displayMode = TOTAL_MODES-1;
  else
    displayMode--;
  
  cleanUp();
}

void goToMode(int8_t mode)
{
  if (0 <= mode && mode < TOTAL_MODES)
  {
    displayMode = mode;
    cleanUp();
  }
}

void cleanUp()
{
  setAllLeds(false);
  clearFrame();
  updateDisplay();
}


void initTouch()
{
  pinMode(pinTRB, INPUT);
  pinMode(pinTRT, INPUT);
  pinMode(pinTLT, INPUT);
  pinMode(pinTLB, INPUT);
  //pinMode(pinPRB, INPUT);
  //pinMode(pinPRT, INPUT);
  //pinMode(pinPLT, INPUT);
  //pinMode(pinPLB, INPUT);
}


byte keysUpDown;
byte keysPressed;
byte prevKeysPressed = 0;

// Read all four buttons and update key states
void readTouch()
{
  byte prev;

  keysPressed = 0;
  keysUpDown = 0;
   
  // BOTTOM LEFT
  prev = prevKeysPressed & BL_KEY_PRESSED_BIT; // Get previous state of the pressed bit
  if (!digitalRead(pinTLB))
  {
    // Button is currently pressed
    keysPressed |= BL_KEY_PRESSED_BIT;
    keysUpDown |= ~prev & BL_KEY_DOWN_BIT; // Button just pressed?
  }
  else
  {
    // Button is currently not pressed
    keysUpDown |= (prev<<4) & BL_KEY_UP_BIT; // Button just released?
  }
 
  // BOTTOM RIGHT
  prev = prevKeysPressed & BR_KEY_PRESSED_BIT;
  if (!digitalRead(pinTRB))
  {
    keysPressed |= BR_KEY_PRESSED_BIT;
    keysUpDown |= ~prev & BR_KEY_DOWN_BIT;
  }
  else
  {
    keysUpDown |= (prev<<4) & BR_KEY_UP_BIT;
  }
  
  // TOP LEFT
  prev = prevKeysPressed & TL_KEY_PRESSED_BIT;
  if (!digitalRead(pinTLT))
  {
    keysPressed |= TL_KEY_PRESSED_BIT;
    keysUpDown |= ~prev & TL_KEY_DOWN_BIT;
  }
  else
  {
    keysUpDown |= (prev<<4) & TL_KEY_UP_BIT;
  }
  
  // TOP RIGHT
  prev = prevKeysPressed & TR_KEY_PRESSED_BIT;
  if (!digitalRead(pinTRT))
  {
    keysPressed |= TR_KEY_PRESSED_BIT;
    keysUpDown |= ~prev & TR_KEY_DOWN_BIT;
  }
  else
  {
    keysUpDown |= (prev<<4) & TR_KEY_UP_BIT;
  }

  prevKeysPressed = keysPressed;
  
}

