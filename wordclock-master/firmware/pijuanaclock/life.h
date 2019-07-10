/* Game of life Arduino code adapted from:
http://forum.arduino.cc/index.php?topic=7091.0
http://pastebin.com/f22bfe94d
*/
#ifndef LIFE_H
#define LIFE_H

#define ANALOG_SEED_PIN A6 // Unconnected pin

void setupGameOfLife();

void gameOfLifeManager(unsigned long now, boolean modeChanged);

void iterateGameOfLife();
 
byte neighbours(int x, int y);

#endif

