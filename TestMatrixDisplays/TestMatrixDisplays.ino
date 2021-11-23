//We always have to include the library
#include "LedControl.h"



int numOfBoards = 3;
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(14,16,15,numOfBoards);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100/numOfBoards;


void single(bool mode);

void setup() {

  Serial.begin(9600);


  for (int i = 0; i < numOfBoards; i++){
   /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
    lc.shutdown(i,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(i,8);
    /* and clear the display */
    lc.clearDisplay(i);
  }

  Serial.print("board");
  Serial.print("\t");
  Serial.print("row");
  Serial.print("\t");
  Serial.println("col");
}

/* 
 This function will light up every Led on the matrix.
*/
void single(bool mode) {
  for(int board=0;board<numOfBoards;board++){
    for(int row=0;row<8;row++) {
      for(int col=0;col<8;col++) {
        delay(delaytime);
        lc.setLed(board,row,col,mode);
        Serial.print(board);
        Serial.print("\t");
        Serial.print(row);
        Serial.print("\t");
        Serial.println(col);
        delay(delaytime);
      }
    }
  }
}

void loop() { 
  lc.setLed(0,0,0,true);
  delay(500);
  lc.setLed(0,0,0,false);
  delay(500);

}
