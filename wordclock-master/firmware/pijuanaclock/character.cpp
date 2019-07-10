// Character functions

#include <avr/pgmspace.h>
#include "character.h"
#include "display.h"


// Adds a character to the frame with the specified offsets
void addCharToFrame(byte offsetX, byte offsetY, byte charIndex )
{
  for (int i = 0; i < CHAR_HEIGHT; i++)
  {
    for (int j = 0; j < CHAR_WIDTH; j++)
    {
      byte x = offsetX + j;
      byte y = offsetY + i;
       
      if (x <= 15 && y <= 15) // Pixel in bounds
      {
         byte charRow = pgm_read_byte(charSet + charIndex*CHAR_HEIGHT + i);
         //frame[y][x] = charRow & (B10000000 >> j);
         boolean b = (charRow & ((byte)B10000000 >> j)) != 0;
         setFrame(frame2,y,x,b);
      }
    }
  }
}

