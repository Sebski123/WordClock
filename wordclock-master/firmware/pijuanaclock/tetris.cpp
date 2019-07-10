#include <avr/pgmspace.h>
#include "character.h"
#include "display.h"
#include "pitches.h"
#include "tetris.h"
#include "touchbuttons.h"

StcGame game;
boolean running = false;

void tetrisManager(unsigned long now, boolean modeChanged)
{
  if (!running) // You can't leave until you finish the game :)
  {
    // Change global mode when top buttons pressed
    if (TL_KEY_DOWN)
    {
      gameEnd(&game); // Free the memory first
      goToPrevMode();
      return;
    }
    if (TR_KEY_DOWN)
    {
      gameEnd(&game);
      goToNextMode();
      return;
    }
  }
  
  // Restart if coming from another mode
  if (modeChanged)
  {
    gameInit(&game); // This does a malloc()
    gameUpdate(&game); // Show a first frame
  }

  // Start game if any bottom button is pressed
  if (!running && !game.isOver && (BL_KEY_DOWN || BR_KEY_DOWN))
    running = true;
  
  if (running)
  {
    gameUpdate(&game);
  }
  soundManager(now, modeChanged);
}  

#define PIN_BUZZER 2

// Tetris melody based on "A Theme - Tetris with Bass" by electricmango (https://electricmango.github.io) 
// License: CC BY-SA 4.0 (https://creativecommons.org/licenses/by-sa/4.0/legalcode)

const int melodyATetris[] PROGMEM = {
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5,  NOTE_B4, NOTE_A4, 
  NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, 
  NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5, NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, 
  NOTE_A3, NOTE_B2,  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5, NOTE_B4, 
  NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5,  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, 
  NOTE_E3, NOTE_A4, 0, // Chorus repeat 2 times
   
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_D4, NOTE_E3, 
  NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3,  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, 
  NOTE_GS2, NOTE_E3,NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3,
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,NOTE_C4, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3,NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,0,0,0,0,0,0,0,0,0,0  // Verse only one time
};

const int melodyBTetris[] PROGMEM = {
  NOTE_E4,0,NOTE_E4,NOTE_E4, NOTE_B3,0, NOTE_B3,0, // Repeat 4 times the introduction
  NOTE_G4,0,NOTE_B4,0,NOTE_E5,0, NOTE_DS5,0,NOTE_E5,0,NOTE_B4,0,NOTE_C4,0,NOTE_A4, 0,NOTE_B4,0,NOTE_G4,0,NOTE_A4,0,NOTE_FS4,0,NOTE_G4,0,NOTE_E4,0,NOTE_FS4,0,NOTE_DS4, 0,NOTE_E4,0,NOTE_B3,0,NOTE_C4,0,NOTE_DS4,0,NOTE_E4,0,NOTE_B3,0,NOTE_C4,0,NOTE_FS4,0,NOTE_E4,
  0,0,NOTE_FS4,0,NOTE_DS4,0,NOTE_E4,0,0,
  // 1
  NOTE_FS4,0,NOTE_G4,0,NOTE_G4,NOTE_G4,NOTE_A4,0,NOTE_FS4,0,NOTE_G4,0,NOTE_G4,NOTE_G4,NOTE_A4,0,NOTE_FS4,0,NOTE_G4,0,0,
  // 2
  NOTE_A4,0,NOTE_B4,0,NOTE_B4,NOTE_B4,NOTE_C4,0,NOTE_A4,0,NOTE_B4,0,NOTE_B4,NOTE_B4,NOTE_C4,0,NOTE_A4,0,NOTE_B4,0,0,
  // 3
  NOTE_CS5,0,NOTE_D5,0,NOTE_D5,NOTE_D5,NOTE_D5,0,NOTE_D5,0,NOTE_E5,0,NOTE_D5,0,NOTE_D5,0,
  NOTE_CS5,0,NOTE_D5,0,NOTE_D5,NOTE_D5,NOTE_D5,0,NOTE_D5,0,NOTE_E5,0,NOTE_D5,0,NOTE_D5,0,
  NOTE_CS5,0,NOTE_D5,0,NOTE_D5,NOTE_D5,NOTE_D5,0,NOTE_D5,0,NOTE_CS5,0,NOTE_CS5,NOTE_CS5,NOTE_CS5,0,NOTE_CS5,0, NOTE_C5,0,NOTE_C5,NOTE_C5,NOTE_C5,0,NOTE_A4,0,NOTE_FS4,0,
  // 4
  NOTE_G4,0,NOTE_A4,0,NOTE_D4,0,
  NOTE_A4,0,NOTE_G4,0,NOTE_D4,0,NOTE_G4,0, NOTE_D4,NOTE_E4,NOTE_D4,0,NOTE_B4,0,
  NOTE_A4,0,NOTE_G4,0,NOTE_D4,0,NOTE_G4,0, NOTE_D4,NOTE_E4,NOTE_D4,0,NOTE_AS4,0,NOTE_D4,0,0,0,0,0
};

/*
const byte noteDurationsATheme[] PROGMEM = {
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,      // Chorus duration. Repeat 2 times

  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8  // Verse duration. Played only one time.
};*/ // Lots of repetitions, optimized away

const byte noteDurationsBTheme[] PROGMEM = {
  16,16,16,16,16,16,16,16,      // Durada Primera estrofa repetida 4 cops [8 Notes]
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16, // Vers repetit 2 cops [51 Notes / Accum: 59]
  16,16,16,16,16,16,16,8,8, // [9 Notes / Accum: 68]
  // 1
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,8,8, // [21 Notes: Accum: 89]
  // 2
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,8,8, // [21 Notes: Accum: 110]
  // 3
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,              // [16 Notes: Accum: 126]
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,              // [16 Notes: Accum: 142]
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16, // [28 Notes: Accum: 170]
  // 4
  16,16,16,16,16,16,                                            // [6 Notes:  Accum: 176]
  16,16,16,16,16,16,16,16, 8,8,8,16,8,16,                       // [14 Notes: Accum: 190]
  16,16,16,16,16,16,16,16, 8,8,8,16,8,16,8,8,8,8,8,8            // [20 Notes: Accum: 210]
};

byte thisNote;
bool repeat;
unsigned int currentDelayTetrisSound;


void setupTetrisSound()
{
  thisNote = 0;
  repeat = true;
  currentDelayTetrisSound = 0;
}

#define TETRIS_MELODY_BASE_TIME 1100

void iterateTetrisSound()
{
  static boolean playing = false;
  if (running)
  {
    //int noteDuration = TETRIS_MELODY_BASE_TIME / pgm_read_byte(noteDurations + thisNote);
    int noteDuration;
    if ( thisNote == 5  ||
         thisNote == 6  ||
         thisNote == 37 ||
         thisNote == 38 ||
         thisNote == 46 ||
         thisNote == 47 )
      noteDuration = TETRIS_MELODY_BASE_TIME /16;
    else if ( thisNote == 34 ||
              thisNote == 62 ||
              thisNote == 63 )
      noteDuration = TETRIS_MELODY_BASE_TIME /4;
    else
      noteDuration = TETRIS_MELODY_BASE_TIME/8;

    if (!playing)
    {
      int note =  pgm_read_word(melodyATetris + thisNote);
      if (note > 0)
        tone(PIN_BUZZER, note); // Start playing the note
      playing = true;
      currentDelayTetrisSound = noteDuration; // Wait for the duration
    }
    else // playing
    {
      noTone(PIN_BUZZER); // Stop playing the note
      playing = false;
      
      thisNote++; // Go to the next note
      if(repeat)
      {
        if (thisNote == 64)   // Chorus melody repeats 2 times
        {
          thisNote = 0;
          repeat = false;
        }
      }
      else //repeat == false
      {
        if (thisNote == 138)  // Verse melody plays only one time
        {
          thisNote = 0;
          repeat = true;
        }
      }
      
      currentDelayTetrisSound = noteDuration * 0.45; // Wait for a little bit // TODO see if we can do without the floating point
    }
  }
  else
    noTone(PIN_BUZZER);
}

void soundManager(unsigned long now, boolean modeChanged)
{
  static unsigned long prev;
  
  if (modeChanged) // Initialize if coming from another mode
  {
    setupTetrisSound();
  }
  
  // Proceed only if just entered this mode or if variable interval has passed
  if (!modeChanged && now-prev < currentDelayTetrisSound)
    return;
  prev = now;

  iterateTetrisSound();
}



//define these for the arduino platform
int platformInit(StcGame *gameInstance){
  return 0;
}

void platformEnd(StcGame *gameInstance) {
}

//modify this to read controller inputs
void platformReadInput(StcGame *gameInstance){
  
  // Top left button moves down the piece
       if (TL_KEY_DOWN) gameOnKeyDown(&game, EVENT_MOVE_DOWN);
  else if (TL_KEY_UP)   gameOnKeyUp  (&game, EVENT_MOVE_DOWN);

  // Top right button rotates the piece  
       if (TR_KEY_DOWN) gameOnKeyDown(&game, EVENT_ROTATE_CW);
  else if (TR_KEY_UP)   gameOnKeyUp  (&game, EVENT_ROTATE_CW);
  
  // Bottom button move the piece left or right
       if (BL_KEY_DOWN) gameOnKeyDown(&game, EVENT_MOVE_LEFT);
  else if (BL_KEY_UP)   gameOnKeyUp  (&game, EVENT_MOVE_LEFT);

       if (BR_KEY_DOWN) gameOnKeyDown(&game, EVENT_MOVE_RIGHT);
  else if (BR_KEY_UP)   gameOnKeyUp  (&game, EVENT_MOVE_RIGHT);  
  
}

//modify this to render the game 
void platformRenderGame(StcGame *gameInstance) {
  char i,j;
  
  if (!game.stateChanged) return; // Nothing changed
  
  if (!game.isPaused && !game.isOver) {
    //draw the static blocks
    for (i = 0; i < BOARD_TILEMAP_WIDTH; i++)
    {
      for (j = 0; j < BOARD_TILEMAP_HEIGHT; j++)
      {
        //frame[BOARD_Y+j][BOARD_X+i] = game.map[i][j];
        setFrame(frame2, BOARD_Y+j, BOARD_X+i, game.map[i][j]);
      }
    }

    // falling piece block
    for (i = 0; i < TETROMINO_SIZE; i++)
    {
      for (j = 0; j < TETROMINO_SIZE; j++)
      {
        // Light up this block only if it was not already lit
        //if (!frame[BOARD_Y+game.fallingBlock.y+j][BOARD_X+game.fallingBlock.x+i])
        if ( !getFrame(frame2,
                BOARD_Y + game.fallingBlock.y + j,
                BOARD_X + game.fallingBlock.x + i) &&
                game.fallingBlock.cells[i][j])
          
          //frame[BOARD_Y+game.fallingBlock.y+j][BOARD_X+game.fallingBlock.x+i] = true;
          setFrame(frame2,
            BOARD_Y + game.fallingBlock.y + j,
            BOARD_X + game.fallingBlock.x + i,
            true);
      }
    }

    // next piece block
    for (i = 0; i < TETROMINO_SIZE; i++)
    {
      for (j = 0; j < TETROMINO_SIZE; j++)
      {
        //frame[NXT_Y+game.nextBlock.y+j+1][NXT_X+game.nextBlock.x + i+1] = game.nextBlock.cells[i][j];
        setFrame(frame2,
          NXT_Y + game.nextBlock.y + j + 1,
          NXT_X + game.nextBlock.x + i + 1,
          game.nextBlock.cells[i][j]);
      }
    }
  
    //update the stats
    // Show level number with 1 character
    addCharToFrame(LEVEL_X, LEVEL_Y, game.stats.level);

    updateDisplay();
  }
  
  if (game.isOver) // Game over
  { 
    running = false;
  }
  
  game.stateChanged = 0;
}

long platformGetSystemTime() {
  return millis();
}

void platformSeedRandom(long seed) {
  srand(analogRead(ANALOG_SEED_PIN)*seed);
}

int platformRandom() {
  return rand();
}

