#include <Wire.h>
#include "time.h"
#include "timeanalog.h"
#include "touchbuttons.h"
#include "display.h"
#include "character.h"
#include "credits.h"

Chronodot RTC;

#define UPDATE_RATE_SETTING_TIME 250 

byte h, m;
byte timeMode = TIME_MODE_NORMAL;

boolean blink = true; // Used to blink the colon or the hour/minute

void timeManager(unsigned long now, boolean modeChanged)
{
  static unsigned long prev;
 
  if (timeMode == TIME_MODE_NORMAL) // Do not change mode when setting time
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
    
    // Enter setting time mode when pressing both bottom buttons
    if ((BL_KEY_DOWN && BR_KEY_PRESSED ||
         BR_KEY_DOWN && BL_KEY_PRESSED ) &&
         displayMode == MODE_DIGITAL_CLOCK) // Allow to set time only in digital mode
    {
      timeMode = TIME_MODE_SETTING_HOUR;
      return;
    }
  }
  else //timeMode != TIME_MODE_NORMAL (Setting time)
  {
    // Exit setting time mode when pressing right bottom button
    if (BR_KEY_DOWN)
    {
      updateRTCtime();
      timeMode = TIME_MODE_NORMAL;
      return;
    }
  }
  
  // Proceed only if just entered this mode or if interval has passed or if setting time
  if (!modeChanged && now-prev < TIME_INTERVAL && timeMode == TIME_MODE_NORMAL)
    return;
   //prev = now; // done below
  
  if (timeMode == TIME_MODE_NORMAL)
  {
    // Get the time from the RTC
    getRTCtime();
    //DEBUG
    /*m++;
    if (m == 60)
    {
      m=0;
      h++;
      if (h == 24)
        h=0;
    }
    */
  }
  else //timeMode != TIME_MODE_NORMAL (Setting time)
  {
    if (BL_KEY_DOWN) // Bottom left button pressed
    {
      if (timeMode == TIME_MODE_SETTING_HOUR) // Finish setting hour, switch to setting minute
      {
        timeMode = TIME_MODE_SETTING_MINUTE;
      }
      else // Finish setting minute, update RTC
      {
        updateRTCtime();
        timeMode = TIME_MODE_NORMAL;
      }  
    }
    else if (TL_KEY_DOWN) // Top left button pressed
    {
       // Decrement hour or minute
       if (timeMode == TIME_MODE_SETTING_HOUR)
       {  
          if (h == 0) h = 23;
          else h--;
       }
       else //timeMode == TIME_MODE_SETTING_MINUTE
       {
          if (m == 0) m = 59;
          else m--;
       }
       blink = true;
    }
    else if (TR_KEY_DOWN)  // Top right button pressed
    {
       // Increment hour or minute
       if (timeMode == TIME_MODE_SETTING_HOUR)
       {  
          if (h == 23) h = 0;
          else h++;
          }
       else //timeMode == TIME_MODE_SETTING_MINUTE
       {
          if (m == 59) m = 0;
          else m++;
       }
       blink = true;
    }
  }
   
  // Show the time on the display
  if (now-prev < UPDATE_RATE_SETTING_TIME && timeMode != TIME_MODE_NORMAL) // Hack to avoid seizure-inducing blinking when setting time
    return;
  prev = now;
  showTime();  
}

void initRTC()
{
  Wire.begin();
  RTC.begin();
  if (!RTC.isrunning())
  {
    //Serial.println(F("[WARNING] RTC is NOT running!"));
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    //RTC.adjust(DateTime("Sep 12 2015", "17:00:00"));
    //RTC.adjust(DateTime(0l));
  }
}

// Fill h and m variables with the current time from the RTC
void getRTCtime()
{
    DateTime now = RTC.now();  
    h = now.hour();
    m = now.minute();
    t = now.tempC();
}


// Update time in the RTC using h and m variables
void updateRTCtime()
{
    DateTime newDateTime = DateTime(
      2015, 9, 12,
      h, m,
      0, 32, 0.0);
    RTC.adjust(newDateTime); 
}


// Show the time with the currently selected clock mode
void showTime()
{
  clearFrame();
  
  switch (displayMode)
  {
    case MODE_WORD_CLOCK:
      showTimeWords();
      break;
    case MODE_DIGITAL_CLOCK:
      showTimeDigits();
      break;
    case MODE_ANALOG_CLOCK:
      showTimeAnalog();
      break;
  }
}



// Show the current time on the display using digits
void showTimeDigits()
{

  byte hTens = h / 10;
  byte hSingles = h % 10;
  byte mTens = m / 10;
  byte mSingles = m % 10;
  
  // Print the hour (blink if setting hour)
  if (timeMode == TIME_MODE_NORMAL ||
      timeMode == TIME_MODE_SETTING_HOUR && blink || 
      timeMode == TIME_MODE_SETTING_MINUTE)
  {
    addCharToFrame(0           ,0            , hTens);
    addCharToFrame(CHAR_WIDTH+1,0            , hSingles);
  }
  
  // Print the minute (blink if setting minute)
  if (timeMode == TIME_MODE_NORMAL ||
      timeMode == TIME_MODE_SETTING_HOUR ||
      timeMode == TIME_MODE_SETTING_MINUTE && blink)
  {
    addCharToFrame(0           ,CHAR_HEIGHT+1, mTens);
    addCharToFrame(CHAR_WIDTH+1,CHAR_HEIGHT+1, mSingles);
  }
  
  // Draw blinking colon
  //frame[2][13] = blink;
  //frame[4][13] = blink;  
  setFrame(frame2,2,13,blink);
  setFrame(frame2,4,13,blink);
  blink = !blink;

  updateDisplay();
}



// Show the current time on the display using words
void showTimeWords()
{
  byte h2 = h;

  // Show "IT IS"
  addWordToFrame(w_it);
  addWordToFrame(w_is);
  
  // Minutes
  if (m == 0) {
    
    if (h == 0) {
      addWordToFrame(w_midnight);
    } else if (h == 12) {
      addWordToFrame(w_noon);
    } else {
      addWordToFrame(w_oclock);
    }

  } else {
  
    if (m <= 20) {
      addWordToFrame(w_minutes[m - 1]);
    } else if (m < 30) {
      addWordToFrame(w_minutes[19]); // twenty
      addWordToFrame(w_minutes[m - 21]);
    } else if (m == 30) {
      addWordToFrame(w_half);
    } else if (m < 40) {
      addWordToFrame(w_minutes[19]); // twenty
      addWordToFrame(w_minutes[60 - m - 21]);
    } else {
      addWordToFrame(w_minutes[60 - m - 1]);
    }
 
    if(m <= 30) {
      addWordToFrame(w_past);
    } else {
      addWordToFrame(w_to);
      ++h2;
    }
    
  } 
  
  if(!(m ==0 && (h == 0 || h == 12))) {
  
    // Hours
    if(h2 == 0) {
      addWordToFrame(w_hours[11]);
    } else if (h2 <= 12) {
      addWordToFrame(w_hours[h2 - 1]);
    } else {
      addWordToFrame(w_hours[h2 - 13]);
    }
    if(h2 == 11 || h2 == 23) {
      addWordToFrame(w_el);
    }
  
    // Time of day
    if(h < 6) {
      addWordToFrame(w_at);
      addWordToFrame(w_night);
    } else if(h < 12) {
      addWordToFrame(w_in);
      addWordToFrame(w_the);
      addWordToFrame(w_morning);
    } else if(h < 18) {
      addWordToFrame(w_in);
      addWordToFrame(w_the);
      addWordToFrame(w_afternoon);
    } else if(h < 22) {
      addWordToFrame(w_in);
      addWordToFrame(w_the);
      addWordToFrame(w_evening);
    } else {
      addWordToFrame(w_at);
      addWordToFrame(w_night);
    }
    
  }
  
  // Temperature
  addWordToFrame(w_and);
  if(t <= 16) {
    addWordToFrame(w_cold);
  } else if (t <= 20) {
    addWordToFrame(w_cool);
  } else if (t <= 30) {
    addWordToFrame(w_warm);
  } else {
    addWordToFrame(w_hot);
  }


  /* 
  #pragma region
  // Minuts
  if (m == 0) {
    afegirEsSon(h,m);
    afegirLaLesHores(h);
    addWordToFrame(w_h_en);
    addWordToFrame(w_h_punt); 
    
  } else { //Minuts són diferents de 0
    if (m < 7) {
      afegirEsSon(h,m);
      afegirLaLesHores(h);
      addWordToFrame(w_i_p_min);
      addWordToFrame(w_minuts_passen[m - 1]);
      if (m == 1) {  
        addWordToFrame(w_p_minut);
      } else {
        addWordToFrame(w_p_minuts);
      }
    } else {
      h2++; // Hora següent
      if (m == 7) {
        afegirEsSon(h2,m);
        addWordToFrame(w_mig_q);
        addWordToFrame(w_quart);   
        afegirDpelsQuarts(h2);
      } else if (m >= 8 && m <= 14) {   // Falten minuts per un quart
        if (m == 14) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-8]);
        addWordToFrame(w_per);
        addWordToFrame(w_a);    
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        afegirDpelsQuarts(h2); 
      } else if (m == 15) {              // Un quart
        afegirEsSon(h2,m);
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        afegirDpelsQuarts(h2);  
      } else if (m >= 16 && m <= 21) {    // Un quart i
        afegirEsSon(h2,m);
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        afegirDpelsQuarts(h2);
        addWordToFrame(w_i_p_min);
        addWordToFrame(w_minuts_passen[m-16]);
        if (m == 16) {
          addWordToFrame(w_p_minut); 
        } else {
          addWordToFrame(w_p_minuts); 
        }
      } else if (m == 22) {              // Un quart i mig
        afegirEsSon(h2,m);
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        addWordToFrame(w_im);
        addWordToFrame(w_mig);
        afegirDpelsQuarts(h2); 
      } else if (m >= 23 && m <= 29) {  // Falten minuts per dos quarts
        if (m == 29) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-23]);
        addWordToFrame(w_per);
        addWordToFrame(w_a);  
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts); 
        afegirDpelsQuarts(h2); 
      } else if (m == 30) {              // Dos quarts
        afegirEsSon(h2,m);
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts); 
        afegirDpelsQuarts(h2);
      } else if (m >= 31 && m <= 36) {   // Dos quarts i
        afegirEsSon(h2,m);
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);
        addWordToFrame(w_i_p_min);
        addWordToFrame(w_minuts_passen[m-31]);
        if (m == 31) {
          addWordToFrame(w_p_minut); 
        } else {
          addWordToFrame(w_p_minuts); 
        }
      } else if (m == 37) {
        afegirEsSon(h2,m);
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts);
        addWordToFrame(w_im);
        addWordToFrame(w_mig);
        afegirDpelsQuarts(h2);
      } else if (m >= 38 && m <= 44) { // Falten minuts per tres quarts
        if (m == 44) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-38]);
        addWordToFrame(w_per);
        addWordToFrame(w_a); 
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);  
      } else if (m == 45) {            // Tres quarts
        afegirEsSon(h2,m);
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);
      } else if (m >= 46 && m <= 51) { // Tres quarts i
        afegirEsSon(h2,m);
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);
        addWordToFrame(w_i_p_min);
        addWordToFrame(w_minuts_passen[m-46]);
        if (m == 46) {
          addWordToFrame(w_p_minut); 
        } else {
          addWordToFrame(w_p_minuts); 
        }
      } else if (m == 52) {
        afegirEsSon(h2,m);
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        addWordToFrame(w_im);
        addWordToFrame(w_mig);
        afegirDpelsQuarts(h2);
      } else if (m >= 53 && m <= 59) {
        if (m == 59) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-53]);
        addWordToFrame(w_per);
        addWordToFrame(w_a); 
        afegirLaLesHores(h2);
      }
    }
  } 
  
  // Hores
  if (h2 == 0) {
    addWordToFrame(w_hores[11]);
  } else if (h2 <= 12) {
    addWordToFrame(w_hores[h2 - 1]);
  } else {
    addWordToFrame(w_hores[h2 - 13]);
  }
  // Matí, migdia, tarda o nit
  if (h >= 6 && h < 12) {
    addWordToFrame(w_del);
    addWordToFrame(w_mati);
  } else if (h >= 12 && h < 15) {
    addWordToFrame(w_del);
    addWordToFrame(w_migdia);
  } else if (h >= 15 && h < 21) {
    addWordToFrame(w_boot_de);
    addWordToFrame(w_boot_la); 
    addWordToFrame(w_tarda);
  } else {
    addWordToFrame(w_boot_de);
    addWordToFrame(w_boot_la);
    addWordToFrame(w_nit);
  }
  #pragma endregion
  */ 
  updateDisplay();
}
