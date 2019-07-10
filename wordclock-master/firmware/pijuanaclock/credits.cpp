#include "brightness.h"
#include "credits.h"
#include "display.h"
#include "touchbuttons.h"


// Show the boot message
void showBootMessage() 
{
  addWordToFrame(w_boot_josep);
  addWordToFrame(w_boot_i);
  addWordToFrame(w_boot_ana);

  updateDisplay();     
  delay(1000);
  
  addWordToFrame(w_boot_12);
  addWordToFrame(w_boot_set);
  addWordToFrame(w_boot_2015);
  
  updateDisplay();      
  delay(2000);

  //setAllLeds(false);
  clearFrame();
  addWordToFrame(w_boot_dels);
  addWordToFrame(w_boot_de);
  addWordToFrame(w_boot_la);
  addWordToFrame(w_boot_uni);
  updateDisplay();
  delay(2000);
  
  clearFrame();
  updateDisplay();
}

unsigned int currentDelay = 10000;

void creditsManager(unsigned long now, boolean modeChanged)
{
  static unsigned long prev;
  
  // Change global mode when top buttons pressed
  if (TL_KEY_DOWN)
  {
    goToPrevMode();
    return;
  }
  if (TR_KEY_DOWN)
  {
    goToNextMode();
    return;
  }

  // Proceed only if just entered this mode or if variable interval has passed
  if (!modeChanged && now-prev < currentDelay)
    return;
  prev = now;

  if (modeChanged) // Initialize if coming from another mode
  {
    setupCreditsNames();
  }
  
  iterateCreditsNames();
}

byte currentLetterOfName;
byte currentName;
byte currentNameOffset;

void setupCreditsNames()
{
  clearFrame(); 
  updateDisplay();

  setAllLeds(false);
  
  currentLetterOfName = 0;
  currentName = 0;
  currentNameOffset = 0;
  currentDelay = DELAY_AFTER_LETTER;
}

void iterateCreditsNames() 
{
  static boolean nameFinished = false;
  
  byte numLettersOfCurrentName = pgm_read_byte(numLettersByName+currentName);
  
  if (nameFinished) 
  {
    nameFinished = false;
    setAllLeds(false);
  }

  if (currentName < NUM_NAMES)
  {
    if (currentLetterOfName < numLettersOfCurrentName*2) // There are 2 bytes (row, column) per letter
    {
      byte row = pgm_read_byte(letters + currentNameOffset + currentLetterOfName);
      byte col = pgm_read_byte(letters + currentNameOffset + currentLetterOfName + 1);

      setLed(row, col, true);

      currentLetterOfName = currentLetterOfName + 2;
      
      currentDelay = DELAY_AFTER_LETTER;
    }
    else // Finished a name
    {
      nameFinished = true;
      
      currentLetterOfName = 0;
      currentNameOffset += numLettersOfCurrentName*2;
      currentName++;

      currentDelay = DELAY_AFTER_NAME;
    }
  } 
  else // Finished, reset
  {
    setupCreditsNames();
  }
}

