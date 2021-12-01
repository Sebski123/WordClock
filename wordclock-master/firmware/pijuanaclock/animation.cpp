#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "animation.h"
#include "touchbuttons.h"
#include "display.h"
#include "serial.h"

// Variables used to detect oscillation and restart
byte currentPos = 0;
boolean ledState = false;
byte animation = 0;
byte speed = 0;
byte mod = 1;
byte frame = 0;
byte dir = 1; //up 0, right 1, down 2, left 3
byte snake[7] = {0, 0, 0, 0, 0, 0, 0};
byte dirArr[4] = {0, 1, 2, 3};
boolean isSnaekDead;

unsigned long previousTime;

void animationManager(unsigned long now, boolean modeChanged)
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

  if (BL_KEY_DOWN) // Change animation
  {
    animation = (animation + 1) % 4;
    setupAnimation();
  }
  if (BR_KEY_DOWN) //Change speed
  {
    if (speed == 0)
    {
      speed = 25;
      return;
    }
    speed = (speed * 2) % 400;
  }

  if (modeChanged) // Restart if coming from another mode
    setupAnimation();

  iterateAnimation();
}

void setupAnimation()
{
  setAllLeds(false);
  currentPos = 0;
  ledState = false;
  previousTime = millis();
  isSnaekDead = false;
  dir = 1;
  memset(snake,0,sizeof(snake));
}

void iterateAnimation()
{
  switch (animation)
  {
  case 0:
    animation1();
    break;
  case 1:
    animation2();
    break;
  case 2:
    animation3();
    break;
  case 3:
    animation4();
    break;
  }
}

void animation1() //Printer fill
{
  if (millis() - previousTime > speed)
  {
    previousTime = millis();
    if (currentPos == 0)
    {
      ledState = !ledState;
    }
    setLed(currentPos / 16, currentPos % 16, ledState);
    currentPos++;
  }
}
void animation2() //Diagonal printer fill
{
  if (millis() - previousTime > speed)
  {
    previousTime = millis();
    if (currentPos == 0)
    {
      mod = (mod + 1) % 7;
      if (mod == 0)
      {
        mod = 2;
      }
      setAllLeds(false);
    }
    if ((currentPos / 16 + currentPos % 16) % mod == 0)
    {
      setLed(currentPos / 16, currentPos % 16, true);
    }
    currentPos++;
  }
}
void animation3() //Square fill
{

  if (millis() - previousTime > speed)
  {
    previousTime = millis();

    if (currentPos == 0)
    {
      Serial.println("flip");
      ledState = !ledState;
    }

    setLed(currentPos / 16, currentPos % 16, ledState);

    if (currentPos == 135)
    {
      dir = 5;
    }
    else if (currentPos == 0)
    {
      dir = 1;
    }
    else if ((currentPos - 16) / 16 == currentPos % 16 && (currentPos / 16) + (currentPos % 16) < 15)
    {
      dir = 1;
    }
    else if (currentPos / 16 + currentPos % 16 == 15 && (currentPos / 16) < 8)
    {
      dir = 2;
    }
    else if (currentPos / 16 == currentPos % 16 && (currentPos / 16) + (currentPos % 16) > 15)
    {
      dir = 3;
    }
    else if (currentPos / 16 + currentPos % 16 == 15 && (currentPos / 16) > 7)
    {
      dir = 0;
    }
    else if (currentPos == 136)
    {
      dir = 5;
    }
    currentPos = moveCurrentPosDir(dir);
  }
}
void animation4() //snaek
{
  if (millis() - previousTime > speed)
  {
    if (!isSnaekDead)
    {
      byte tempPos;
      previousTime = millis();
      shuffleArr(dirArr);

      for (size_t i = 0; i < 4; i++)
      {
        tempPos = moveCurrentPosDir(dirArr[i]);
        if (isPosInSnake(tempPos))
        {
          continue;
        }
        shiftSnakeArr(tempPos);
        currentPos = tempPos;
        drawSnake();
        return;
      }
      Serial.println("HElloooo");
      isSnaekDead = true;
    }
    else if(millis()-previousTime > 2000);
    {
      setupAnimation();
    }
  }
}

void shuffleArr(byte arr[])
{
  for (size_t i = 0; i < 4; i++)
  {
    byte n = random(0, 4);
    byte temp = arr[n];
    arr[n] = arr[i];
    arr[i] = temp;
  }
  
}

byte moveCurrentPosDir(byte dir)
{
  byte tempPos = currentPos;
  switch (dir)
  {
  case 0: //up
    tempPos = tempPos - 16;
    break;

  case 1: //right
    tempPos++;
    break;

  case 2: //down
    tempPos = tempPos + 16;
    break;

  case 3: //left
    tempPos--;
    break;

  case 5:
    tempPos = 0;
    dir = 1;
    break;
  }
  return tempPos;
}

boolean isPosInSnake(byte pos)
{
  for (size_t i = 0; i < 7; i++)
  {
    if (snake[i] == pos)
    {
      return true;
    }
  }
  return false;
}

void drawSnake()
{
  setLed(snake[0] / 16, snake[0] % 16, true);
  setLed(snake[1] / 16, snake[1] % 16, true);
  setLed(snake[2] / 16, snake[2] % 16, true);
  setLed(snake[3] / 16, snake[3] % 16, true);
  setLed(snake[4] / 16, snake[4] % 16, true);
  setLed(snake[5] / 16, snake[5] % 16, true);
  setLed(snake[6] / 16, snake[6] % 16, true);
}

void shiftSnakeArr(byte newPos)
{
  setLed(snake[6] / 16, snake[6] % 16, false);
  for (size_t i = 6; i > 0; i--)
  {
    snake[i] = snake[i-1];
  }
  snake[0] = newPos;
}