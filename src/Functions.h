#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Adafruit_BMP280.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

// pins & globals
extern const int buzzerPin;
extern const int trigPin;
extern const int echoPin;
extern int selectedMenuItem;
extern int playerLevel;
extern Adafruit_ILI9341 tft;
extern Adafruit_BMP280 bmp; 

// your existing prototypes
void drawRandomHex(int lines);
void bootCheckSequence();
void wipeTransition(uint16_t bgColor);
void showWelcome();
void drawMainMenu(int selectItem);
void playPipBoyBootSound();

// ---- add these ----
long readDistance();                  // sensor helper
void drawDistanceScreen();            // draws the static frame
void updateDistanceValue();           // updates the number in place

#endif // FUNCTIONS_H
