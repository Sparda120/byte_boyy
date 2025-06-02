#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Adafruit_ILI9341.h>
#include <Arduino.h>

extern const int buzzerPin;
extern const int trigPin;
extern const int echoPin;
extern int playerLevel;
extern int playerXP;
extern int xpToNextLevel;


// Declare your functions here (function prototypes)
void drawRandomHex(int lines);
void bootCheckSequence();
void wipeTransition(uint16_t bgColor);
void showWelcome();
void drawMainMenu(int selectItem);
void playPipBoyBootSound();
void addXP(int amount);
void drawLevelInfo();
void drawLevelBarOutline(int level);
void updateXPBarFill(int currentXP, int maxXP);



#endif