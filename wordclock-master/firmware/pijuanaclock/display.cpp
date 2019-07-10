#include "display.h"

//boolean     frame[SIZE][SIZE];
//boolean prevframe[SIZE][SIZE];

// New space-saving frame
byte     frame2[32];
byte prevFrame2[32];

 
// The led controllers (MAX7219)
LedControl lc = LedControl(pinData, pinClock, pinLoad, noBoards);

void initDisplay()
{
  for (int i = 0; i < noBoards; ++i)
  {
    lc.shutdown(i, false);
    lc.clearDisplay(i);
  }
}

void setBrightness(int value)
{
  for (int i = 0; i < noBoards; ++i)
    lc.setIntensity(i, value);
}

// Turn all LEDs on or off
void setAllLeds(boolean state)
{
  for (int i = 0; i < noBoards; ++i)
    if (state)
      for (int r = 0; r < 8; ++r)
        lc.setRow(i, r, B11111111);
    else
        lc.clearDisplay(i);
}


// LED grid 
#define bs  8
#define bss 3

// Switch on or off a LED, applying the required transform
void setLed(byte row, byte col, boolean on)
{
  const byte boards[2][2] = { {2, 1}, {3, 0} };
  
  byte t = row >> bss;
  byte l = col >> bss;
  byte board = boards[t][l];
  byte r = row % bs;
  byte c = col % bs;
  if( t == 0)
  {
    if (l == 0)
    {
      r = bs - 1 - r;
      c = (c + 1) % bs;
    }
    else
    {
      int r1 = r;
      r = c;
      c = (r1 + 1) % bs;
    }
  }
  else
  {
    if (l == 0)
    {
      int r1 = r;
      r = bs - 1 - c;
      c = (bs - r1) % bs;
    }
    else
    {
      c = (bs - c) % bs;
    }
  }
  lc.setLed(board, r, c, on);
}


// Frame functions

// Set a LED in the frame on or off
void setFrame(byte* frame2, byte row, byte col, boolean on)
{
  byte byteNum = 2*row + col/8;
  byte posInByte = col%8;

  byte mask = (byte)B10000000 >> posInByte;
  
  if (on)
    frame2[byteNum] |= mask;
  else
    frame2[byteNum] &= ~mask;
}

// Get the state of an LED in the frame
boolean getFrame(byte* frame2, byte row, byte col)
{
  byte byteNum = 2*row + col/8;
  byte posInByte = col%8;
  
  return (frame2[byteNum] & ((unsigned char)B10000000 >> posInByte)) != 0;
}
 
// Show the frame on the display (only switch changed LEDs)
// Also copy the frame to the previous frame buffer
void updateDisplay()
{
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
    {
      boolean b = getFrame(frame2,r,c);
      
      if (getFrame(prevFrame2,r,c) != b)
      {
        setFrame(prevFrame2,r,c,b);
        setLed(r, c, b);
      }
    }
}

/*
void updateDisplay()
{
  for (int r = 0; r < SIZE; ++r)
    for (int c = 0; c < SIZE; ++c)
      if (prevframe[r][c] != frame[r][c])
      {
        prevframe[r][c] = frame[r][c];
        setLed(r, c, frame[r][c]);
      }
}
*/

// Clear the frame
void clearFrame()
{
  //memset(frame, 0, sizeof(frame));
  memset(frame2, 0, sizeof(frame2));  
}


// Add a word to the frame
void addWordToFrame(const byte theword[3])
{
  byte len = pgm_read_byte(theword+2);
  byte row, col;
  for (int i = 0; i < len; ++i)
  {
    row = pgm_read_byte(theword);
    col = pgm_read_byte(theword + 1);
    //frame[row][col+i] = true;
    setFrame(frame2,row,col+i,true);
  }
}

/*
// Animation functions

// Add an animation sprite to the frame with the specified offsets (>1s, not smooth)
void addSpriteToFrame(int offsetX, int offsetY, const byte animSprite[32])
{
  int x, y;
  for (int i = 0; i < 32; i++) // Iterate sprite slices
  {
    y = offsetY + i/2; // Row from 0 to 15
    if (0 <= y && y <= 15) // Row in bounds
    {
      byte slice = pgm_read_byte(animSprite + i); 
      for (int b = 0; b < 8; b++) // Iterate bits on the slice
      {
        x = offsetX + (i%2)*8 + b; // even slices are on the left, odd on the right, plus bit number from the left (MSB)
      
        if (0 <= x && x <= 15) // Column in bounds
          //frame[y][x] = slice & (B10000000 >> b);
          setFrame(frame2,y,x,slice & (B10000000 >> b));
      }
    }
  }
}

// Show an animation sprite with the specified offsets (bypassing frame) (~302ms max, depending on offsets)
void showAnimSprite(int offsetX, int offsetY, const byte animSprite[32])
{
  int x, y;
  for (int i = 0; i < 32; i++) // Iterate sprite slices
  {
    y = offsetY + i/2; // Row from 0 to 15
    if (0 <= y && y <= 15) // Row in bounds
    {
      byte slice = pgm_read_byte(animSprite + i);
      for (int b = 0; b < 8; b++) // Iterate bits on the slice
      {
        x = offsetX + (i%2)*8 + b; // even slices are on the left, odd on the right, plus bit number from the left (MSB)
      
        if (0 <= x && x <= 15) // Column in bounds
          setLed(y, x, slice & (B10000000 >> b));
      }
    }
  }
}

// Show a pretransformed animation sprite (~37ms constant)
void showAnimSpritePT(const byte animSprite[32])
{
  for (int i = 0; i < 32; i++)
    lc.setRow(i/8, i%8, animSprite[i]);
}



// Animation test
void animSmileys()
{
  clearFrame();
  for(int i = -16; i < 16; i++)
  {
    addSpriteToFrame(i,i,smileys); // Scroll sprite diagonally, top left to bottom right
    updateDisplay();
    
    //delay(50);
  }
  
  delay(1000);
  
  clearFrame();
  for(int i = -16; i < 16; i++)
  {
    showAnimSprite(i,i,smileys); // Test without using frame
    //delay(50);
  }
}

// Prova de velocitat
void testSpeed()
{
  unsigned long start, end;
     
  setAllLeds(false);
  
  start = millis();
  
  // Normal, pÃ­xel a pÃ­xel
  for (int r = 0; r < 16; r++)
  {
    for (int c = 0; c < 16; c++)
    {
      setLed(r,c,true);
      //delay(100);
      //setLed(r,c,false);
    }
  }
  end = millis();
  
  Serial.print(F("Normal (ms): ")); // 302ms
  Serial.println(end-start);
  
  setAllLeds(false);
  
  start = millis();
  // Dades pretransformades
  for (int i = 0; i < 32; i++)
    lc.setRow(i/8, i%8, B11111111);
  
  end = millis();
  Serial.print(F("Pretrans (ms): ")); //  37ms
  Serial.println(end-start);
}
*/

