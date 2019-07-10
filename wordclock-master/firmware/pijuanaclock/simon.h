//Simon functions

#ifndef SIMON_H
#define SIMON_H

#define ANALOG_SEED_PIN A6 // Unconnected pin

void setupSimon();

void simonManager(unsigned long now, boolean modeChanged);

void simonUpdate();

void platformReadInput();

void turnOnQuadrant(byte quadrant);

byte whichPosition(byte turn);

#endif

