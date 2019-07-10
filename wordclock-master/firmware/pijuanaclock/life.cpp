#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "life.h"
#include "touchbuttons.h"
#include "display.h"

#define GAME_OF_LIFE_INTERVAL 128

boolean noChange;

// Variables used to detect oscillation and restart
byte prevSignature;
byte signature;
byte signatureMatchCount;
boolean skipIteration;
#define MATCHES_TO_RESTART 25

#define DENSITY 50;
 
void gameOfLifeManager(unsigned long now, boolean modeChanged)
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
  
  if (BL_KEY_DOWN || BR_KEY_DOWN) // Restart when any bottom button is pressed
  {
    setupGameOfLife();
  }
  
  // Proceed only if just entered this mode or if interval has passed
  if (!modeChanged && now-prev < GAME_OF_LIFE_INTERVAL)
    return;
  prev = now;
  
  
  if (modeChanged) // Restart if coming from another mode
    setupGameOfLife();
  
  iterateGameOfLife();
}  
 
void setupGameOfLife() {

  randomSeed(analogRead(ANALOG_SEED_PIN)+millis());
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      //prevframe[i][j] = random(100) < DENSITY;
      boolean b = random(100) < DENSITY;
      setFrame(prevFrame2, i, j, b);
      //frame[i][j] = 0;
      setFrame(frame2, i, j, 0);
    }
  }
  
  signatureMatchCount = 0;
  prevSignature = 0;
  signature = 0;
  skipIteration = false;
}


void iterateGameOfLife() {
  
  signature = 0;
  noChange = true;
  
  // Birth and death cycle
  for (int x = 0; x < SIZE; x++)
  {
    for (int y = 0; y < SIZE; y++)
    {
      // Default is for cell to stay the same
      //frame[x][y] = prevframe[x][y];
      setFrame(frame2,x,y, getFrame(prevFrame2, x, y));
      byte count = neighbours(x, y);
      if (count == 3 && 
        //prevframe[x][y] == 0)
        getFrame(prevFrame2, x, y) == 0)
      {
        // A new cell is born
        //frame[x][y] = 1;
        setFrame(frame2, x, y, 1);

        noChange = false;
        signature += -x-y; // Arbitrary
      }
      if ((count < 2 || count > 3) 
        //&& prevframe[x][y] == 1)
        && getFrame(prevFrame2, x, y) == 1)
      
      {
        // Cell dies
        //frame[x][y] = 0;
        setFrame(frame2, x, y, 0);

        noChange = false;
        signature += x+y;
      }
    }
  }

  // At this point frame contains the current generation and prevframe the previous one.
  // We can call updateDisplay() here so that only changed LEDs are written.
  // It will also copy frame into prevframe.
  updateDisplay();


  // Restart if no change done
  if (noChange)
  {
    delay(1000);
    setupGameOfLife();
    return;
  }
 
  // Oscillator detection
  // Check if the signature of the changes matches the one two steps before
  // If signature matches many times, oscillation should be happening, restart.
  // This should allow gliders to glide, but simple oscillators should be detected
  if (!skipIteration)
  {
    if (signature == prevSignature)
    {
      signatureMatchCount++;
      if (signatureMatchCount == MATCHES_TO_RESTART)
      {
        // Restart
        delay(1000);
        setupGameOfLife();
        return;
      }
    }
    prevSignature = signature;
  }
  skipIteration = !skipIteration;
}


byte neighbours(int x, int y) {
 /*return prevframe[(x + 1) % SIZE][y] +
        prevframe[x][(y + 1) % SIZE] +
        prevframe[(x + SIZE - 1) % SIZE][y] +
        prevframe[x][(y + SIZE - 1) % SIZE] +
        prevframe[(x + 1) % SIZE][(y + 1) % SIZE] +
        prevframe[(x + SIZE - 1) % SIZE][(y + 1) % SIZE] +
        prevframe[(x + SIZE - 1) % SIZE][(y + SIZE - 1) % SIZE] +
        prevframe[(x + 1) % SIZE][(y + SIZE - 1) % SIZE];*/
/*
 return getFrame(prevFrame2,(x + 1) % SIZE,y) +
        getFrame(prevFrame2,x,(y + 1) % SIZE) +
        getFrame(prevFrame2,(x + SIZE - 1) % SIZE,y) +
        getFrame(prevFrame2,x,(y + SIZE - 1) % SIZE) +
        getFrame(prevFrame2,(x + 1) % SIZE,(y + 1) % SIZE) +
        getFrame(prevFrame2,(x + SIZE - 1) % SIZE,(y + 1) % SIZE) +
        getFrame(prevFrame2,(x + SIZE - 1) % SIZE,(y + SIZE - 1) % SIZE) +
        getFrame(prevFrame2,(x + 1) % SIZE,(y + SIZE - 1) % SIZE);*/
//TODO Removed some superfluous operations, check it:
 return 
        getFrame(prevFrame2,  x            , (y + 1) % SIZE) +
        getFrame(prevFrame2,  x            , (y - 1) % SIZE) +
        getFrame(prevFrame2, (x + 1) % SIZE,  y            ) +
        getFrame(prevFrame2, (x + 1) % SIZE, (y + 1) % SIZE) +
        getFrame(prevFrame2, (x + 1) % SIZE, (y - 1) % SIZE) +   
        getFrame(prevFrame2, (x - 1) % SIZE,  y            ) +
        getFrame(prevFrame2, (x - 1) % SIZE, (y + 1) % SIZE) +
        getFrame(prevFrame2, (x - 1) % SIZE, (y - 1) % SIZE);
}


//TODO check if this one is better, it uses less flash 
//Does not work correctly yet
byte neighbours2(int x, int y)
{
  byte count=0;
  for (int i=-1;i<=1;i++)
  {
    for (int j=-1;j<=1;j++)
    {
      if (i!=0 || j!=0)
      {
        byte c = (x+i) % SIZE;
        byte r = (y+j) % SIZE;
        
        //count += prevframe[r][c];
        count += getFrame(prevFrame2,r,c);
      }
    }
  }
  return count;
}

