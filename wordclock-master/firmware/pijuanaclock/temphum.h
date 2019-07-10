// Temperature and humidity sensor functions

#ifndef TEMPHUM_H
#define TEMPHUM_H

#include "DHT.h"  // Temperature and humidity Sensor Library - https://github.com/adafruit/DHT-sensor-library

#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define pinDHT   11 // digital pin 11 for temp & hum sensor

#define TEMP_HUM_INTERVAL 2000

void tempHumManager(unsigned long now, boolean modeChanged);

void initTempHum();

void getTempHum();

void showTempHum();

#endif


