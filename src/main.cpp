#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include "Functions.h"

// Pin definitions for display
#define TFT_CS    10
#define TFT_DC    5
#define TFT_RST   6
#define TRIG_PIN 2
#define ECHO_PIN 3



//Declarar variaveis
int selectedMenuItem = 0; // 0 menu inicial
const int buzzerPin = 8;
int playerLevel = 1;
int playerXP = 0;
int xpToNextLevel = 100;  // XP threshold for next level 


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  tft.begin(); //Inicia o display
  tft.setRotation(1); //Landscape
  tft.invertDisplay(true); //Corrigir as cores do display chines
  pinMode(buzzerPin, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  drawRandomHex(20); //Class de numeros aleatorios no boot
  delay(1000);
  wipeTransition(ILI9341_BLACK);
  bootCheckSequence(); //Check ups ficticios com alguns delays 
  delay(2000);
  wipeTransition(ILI9341_BLACK);
  showWelcome(); //Menu com developers e introducao de disciplina
  delay(5000);
  wipeTransition(ILI9341_BLACK); 
  drawMainMenu(1);
  drawLevelBarOutline(playerLevel);      // Desenha o nivel e a barra do nivel
  updateXPBarFill(playerXP, xpToNextLevel);  // Enche a barra consoante o xp
  
}

void loop() {
 
  }

  

