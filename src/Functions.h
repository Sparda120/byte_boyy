#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Adafruit_BMP280.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

// Variaveis
extern const int buzzerPin;
extern const int trigPin;
extern const int echoPin;
extern int selectedMenuItem;
extern int playerLevel;
extern Adafruit_ILI9341 tft;
extern Adafruit_BMP280 bmp; 

// Referencia de todas as funções
void drawRandomHex(int lines);
void bootCheckSequence();
void wipeTransition(uint16_t bgColor);
void showWelcome();
void drawMainMenu(int selectItem);
void playPipBoyBootSound();
long readDistance();                 
void drawDistanceScreen();            
void updateDistanceValue();     
void playClickSound();      

#endif 
