/* Game of life Arduino code adapted from:
http://forum.arduino.cc/index.php?topic=7091.0
http://pastebin.com/f22bfe94d
*/
#ifndef ANIMATION_H
#define ANIMATION_H

void animationManager(unsigned long now, boolean modeChanged);
void setupAnimation();
void iterateAnimation();
void animation1();
void animation2();
void animation3();
void animation4();
void shuffleArr(byte arr[]);
byte moveCurrentPosDir(byte dir);
boolean isPosInSnake(byte pos);
void drawSnake();
void shiftSnakeArr(byte newPos);

#endif

