#include "temphum.h"
#include "display.h"
#include "character.h"
#include "touchbuttons.h"

// Initialize DHT sensor for normal 16MHz Arduino
DHT dht(pinDHT, DHTTYPE);

byte temp, hum;

void tempHumManager(unsigned long now, boolean modeChanged)
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
  
  // Proceed only if just entered this mode or if interval has passed
  if (!modeChanged && now-prev < TEMP_HUM_INTERVAL)
    return;
  prev = now;
 
  getTempHum();
  showTempHum();
}

void initTempHum()
{
  dht.begin();
}


void getTempHum()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  float humidity  = dht.readHumidity();
  if (isnan(humidity)) 
    hum = 255;
  else
  {
    hum = round(humidity);
    if (hum >= 100) // We only have two digits
      hum = 99;
  }
   
  // Read temperature as Celsius
  // Negative is not supported
  float temperature = dht.readTemperature();
  if (isnan(temperature)) 
    temp = 255;
  else
    temp = round(temperature);
}

void showTempHum()
{
  clearFrame();
  
  if (temp != 255)
  {
    byte tempDesena = temp / 10;
    byte tempUnitat = temp % 10;
    addCharToFrame(0           ,0            , tempDesena);
    addCharToFrame(CHAR_WIDTH+1,0            , tempUnitat);
  }
  else
  {
    addCharToFrame(0           ,0            , 12); // Show "--" in case of error
    addCharToFrame(CHAR_WIDTH+1,0            , 12);
  }
  addCharToFrame(2*(CHAR_WIDTH+1),0            , 10); // Add ° character
  
  if (hum != 255)
  {
    byte humDesena = hum / 10;
    byte humUnitat = hum % 10;
    addCharToFrame(0           ,CHAR_HEIGHT+1, humDesena);
    addCharToFrame(CHAR_WIDTH+1,CHAR_HEIGHT+1, humUnitat);
  }
  else
  {
    addCharToFrame(0           ,CHAR_HEIGHT+1, 12);
    addCharToFrame(CHAR_WIDTH+1,CHAR_HEIGHT+1, 12);
  }
  addCharToFrame(2*(CHAR_WIDTH+1),CHAR_HEIGHT+1, 11); // Add % character
  
  updateDisplay();
}


