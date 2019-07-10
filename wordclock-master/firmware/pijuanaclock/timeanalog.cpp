#include "timeanalog.h"
#include "display.h"
#include "time.h"

void line(int x0, int y0, int x1, int y1){
   int dx = abs(x1-x0);
   int dy = abs(y1-y0);
   int sx = (x0 < x1) ? 1 : -1;
   int sy = (y0 < y1) ? 1 : -1;
   int err = dx-dy;

   while(true){
     //frame[y0][x0] = true;
     setFrame(frame2, y0, x0, true);

     if ((x0==x1) && (y0==y1)) break;
     int e2 = 2*err;
     if (e2 >-dy){ err -= dy; x0  += sx; }
     if (e2 < dx){ err += dx; y0  += sy; }
   }
}


void drawTime(int h,int minutes) {
    float angle = pi - (float)h*pi/6.0f;
    line(7,7,
        round(HOUR_HAND_LENGTH*sin(angle)+7.0f),
        round(HOUR_HAND_LENGTH*cos(angle)+7.0f)
        );

    angle = pi - (float)(minutes/5.0f)*(pi/6.0f);
    line(7,7,
        round(MINUTE_HAND_LENGTH*sin(angle)+7.0f),
        round(MINUTE_HAND_LENGTH*cos(angle)+7.0f)
        );
}

// Show the current time on the display using an analog clock
void showTimeAnalog() {

  // Draw the numbers as points
  setFrame(frame2, 0 , 7, true);  //0
  setFrame(frame2, 1 , 11, true); //5
  setFrame(frame2, 4 , 13, true); //10
  setFrame(frame2, 7 , 14, true); //15
  setFrame(frame2, 10, 13, true); //20
  setFrame(frame2, 13, 11, true); //25
  
  setFrame(frame2, 14, 7, true);  //30
  setFrame(frame2, 13, 3, true);  //35
  setFrame(frame2, 10 , 1, true); //40
  setFrame(frame2, 7 , 0, true);  //45
  setFrame(frame2, 4 , 1, true);  //50
  setFrame(frame2, 1 , 3, true);  //55

  drawTime(h, m);
  
  updateDisplay();
}

