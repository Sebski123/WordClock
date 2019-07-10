#include "touchbuttons.h"
#include "display.h"
#include "character.h"
#include "simon.h"

#define START              0

#define SHOW_SEQUENCE_ON  1
#define SHOW_SEQUENCE_OFF 2

#define WAIT_SEQUENCE_ON  3
#define WAIT_SEQUENCE_OFF 4

#define END              5

#define PIN_BUZZER 2

#define DELAY_SHOW_SEQUENCE_ON    500
#define DELAY_SHOW_SEQUENCE_OFF   500
#define DELAY_ERROR_PUSHED_BUTTON 500

// Quadrants defines
#define UP_LEFT     1
#define UP_RIGHT    2
#define DOWN_LEFT   3
#define DOWN_RIGHT  4

#define MAX_TURNS 64 // Divisible by 4
#define NUM_BYTES_POSITIONS MAX_TURNS/4

byte positionsSimon[NUM_BYTES_POSITIONS]; // Using 1 byte to save the 4 positions

byte turn;
byte numTurnsToShow;

byte state;
boolean runningSimon = false;

unsigned int currentDelaySimon = 1000;

void simonManager(unsigned long now, boolean modeChanged)
{
  static unsigned long prev;

  if (!runningSimon) // You can't leave until you finish the game :)
  {
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
  }

  // Start game if any bottom button is pressed
  if (!runningSimon && (BL_KEY_DOWN || BR_KEY_DOWN))
  {
    if (!runningSimon)
      setupSimon();
    
    runningSimon = true;
    
    state = START;
    //Serial.println(F("SIMON IS RUNNING!!!!!"));
  }

  // Proceed only if just entered this mode or if variable interval has passed
  if (!modeChanged && now - prev < currentDelaySimon)
    return;
  prev = now;

  // Restart if coming from another mode
  if (modeChanged)
  {
    setupSimon();
  }

  if (runningSimon)
    simonUpdate();
}


void setupSimon()
{
  clearFrame(); 
  updateDisplay();

  setAllLeds(false);

  // Simon Title
  setLed(1,3,true);
  setLed(1,4,true);
  setLed(6,2,true);
  setLed(6,5,true);
  setLed(6,8,true);

  delay(1000);

  turn = 0;
  numTurnsToShow = 1;

  state = START;

  randomSeed(analogRead(ANALOG_SEED_PIN) + millis());

  for (int i = 0; i < NUM_BYTES_POSITIONS; i++)
  {
    positionsSimon[i] = random(0, 255);
  }

  //turnOnQuadrant(4);
}

void simonUpdate()
{
  switch (state)
  {
    case START:
    {
      setAllLeds(false);
      state = SHOW_SEQUENCE_ON;
    }  
    break;
    case SHOW_SEQUENCE_ON:
    {
      byte pos = whichPosition(turn);
      //Serial.print(F("MSE Torn: ")); Serial.print(turn);
      //Serial.print(F("   MSE numTurnsToShow: ")); Serial.print(numTurnsToShow); Serial.print(F(" Seq:")); Serial.println(pos);

      turnOnQuadrant(pos);
      currentDelaySimon = DELAY_SHOW_SEQUENCE_OFF;
      tone(PIN_BUZZER, pos * 100); 
      state = SHOW_SEQUENCE_OFF;
    }
    break;
    case SHOW_SEQUENCE_OFF:
    {
      setAllLeds(false);
      noTone(PIN_BUZZER);
      byte pos = whichPosition(turn);
      //Serial.print(F("MSA Torn: ")); Serial.print(turn); Serial.print(F(" Seq:")); Serial.println(pos);

      if (turn == numTurnsToShow - 1) // The full sequence has been shown
      {
        turn = 0;
        currentDelaySimon = 0;  // Do not change, if you do then the next visualization won't work
        state = WAIT_SEQUENCE_ON;
      }
      else
      {
        turn++;
        currentDelaySimon = DELAY_SHOW_SEQUENCE_ON;
        state = SHOW_SEQUENCE_ON;
      }
    }
    break;

    case WAIT_SEQUENCE_ON:
    {
      if (ANY_KEY_DOWN) // A button was just pressed
      {
        byte pos = whichPosition(turn);

        if ((TL_KEY_DOWN && pos == UP_LEFT) ||
            (TR_KEY_DOWN && pos == UP_RIGHT)    ||
            (BL_KEY_DOWN && pos == DOWN_LEFT) ||
            (BR_KEY_DOWN && pos == DOWN_RIGHT     )) // The right key has been pressed
        { 
          turnOnQuadrant(pos);
          //Serial.println(F("ESE Hem encertat"));
          state = WAIT_SEQUENCE_OFF;
          currentDelaySimon = 0;
          tone(PIN_BUZZER, pos * 100); 
        }
        else // A wrong key has been pressed
        {
          turnOnQuadrant(pos);
          state = END;
          //Serial.println(F("ESE Hem fallat"));
          currentDelaySimon = DELAY_ERROR_PUSHED_BUTTON;
          tone(PIN_BUZZER, 80, currentDelaySimon);
          tone(PIN_BUZZER, 70, currentDelaySimon); 
          tone(PIN_BUZZER, 60, currentDelaySimon); 
        }
      }
    }
    break;
    case WAIT_SEQUENCE_OFF: //TODO OFF only with the UP of the correct button
    {
      if (ANY_KEY_UP) 
      { 
        setAllLeds(false);
        noTone(PIN_BUZZER);
              
        if (turn == numTurnsToShow - 1) // All the sequence has been done correctly
        {
          //Serial.println(F(" sequence correct"));
          turn = 0;
          if (numTurnsToShow <= MAX_TURNS) //TODO if numTurnsToShow = MAX_TURNS do something
            numTurnsToShow++;
          state = SHOW_SEQUENCE_ON;
          currentDelaySimon = DELAY_SHOW_SEQUENCE_ON;
        }
        else
        {
          turn++;
          state = WAIT_SEQUENCE_ON;
          currentDelaySimon = 0;
        }
      }
    }  
    break;

    case END:
    {
      setAllLeds(false);
      clearFrame();

      byte pointsTen  = (numTurnsToShow-1) / 10;
      byte pointsUnit = (numTurnsToShow-1) % 10;
      
      if (numTurnsToShow-1 < 10)
      {
        addCharToFrame(5, 5, pointsUnit);
      }
      else
      {
        addCharToFrame(2, 5, pointsTen);
        addCharToFrame(2+CHAR_WIDTH+1, 5, pointsUnit);
      }
        
      updateDisplay();
      
      runningSimon = false;
      //Serial.println(F("SIMON IS NOT RUNNING!"));
    }
    break;
  }
}


byte whichPosition(byte turn) // Which position needs to be turn on
{
  byte posByte = positionsSimon[turn / 4];
  byte residu  = turn % 4;

  posByte = posByte >> (residu * 2);

  posByte = posByte & B00000011;

  return posByte + 1 ;
}


void turnOnQuadrant (byte quadrant)
{
  byte offset_row = 0;
  byte offset_column = 0;

  if (quadrant == 3 || quadrant == 4)
  {
    offset_row = 8;
  }

  if (quadrant == 2 || quadrant == 4)
  {
    offset_column = 8;
  }

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      setLed(i + offset_row,j + offset_column, true);
    }
  }
}


