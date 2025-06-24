#include "Functions.h"
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>

extern Adafruit_ILI9341 tft;
extern const int trigPin;
extern const int echoPin;
extern Adafruit_BMP280 bmp;

// Lê distância em cm do sensor de ultrassom; devolve -1 se não houver eco
long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(500);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 40000UL);
  Serial.print("raw pulseIn: ");
  Serial.print(duration);
  Serial.println(" µs");

  if (duration == 0) {
    return -1;  // sem eco dentro do timeout
  }
  return duration / 58;  // conversão de µs para cm
}

// Desenha o ecrã de distância com moldura, abas e etiquetas
void drawDistanceScreen() {
  tft.fillScreen(ILI9341_BLACK);

  const char* tabs[] = { "TEMP", "DIS", "GAS", "LIGHT" };
  for (int i = 0, x = 15; i < 4; i++, x += 60) {
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_GREEN);
    if (i == 1) {
      int16_t x1, y1; uint16_t w, h;
      tft.getTextBounds(tabs[i], x, 10, &x1, &y1, &w, &h);
      tft.drawRect(x1 - 4, y1 - 2, w + 8, h + 4, ILI9341_GREEN);
    }
    tft.setCursor(x, 10);
    tft.print(tabs[i]);
  }
  tft.drawFastHLine(0, 35, tft.width(), ILI9341_GREEN);

  int imgY = 45;
  tft.drawRect(20, imgY, tft.width() - 40, 130, ILI9341_GREEN);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(30, imgY + 10);
  tft.print("Distance:");

  // Pontos nos cantos do ecrã
  tft.fillRect(0, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width()-10, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(0, tft.height()-10, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width()-10, tft.height()-10, 10, 10, ILI9341_GREEN);
}

// Atualiza apenas o valor numérico da distância no ecrã, apagando o anterior
void updateDistanceValue() {
  long d = readDistance();

  int x = 140, y = 55;
  tft.fillRect(x, y, 80, 16, ILI9341_BLACK);

  tft.setCursor(x, y);
  if (d < 0) {
    tft.print("--");  // fora de alcance
  } else {
    tft.print(d);
    tft.print(" cm");
  }
}

// Desenha o menu principal com separador, abas e pré-visualização conforme selectedItem
void drawMainMenu(int selectedItem) {
  tft.fillScreen(ILI9341_BLACK);

  const char* tabs[] = { "TEMP", "DIS", "GAS", "LIGHT" };
  const int numTabs = sizeof(tabs) / sizeof(tabs[0]);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);

  int tabX = 15;
  for (int i = 0; i < numTabs; i++) {
    if (i == selectedItem) {
      int16_t x1, y1; uint16_t w, h;
      tft.getTextBounds(tabs[i], tabX, 10, &x1, &y1, &w, &h);
      tft.drawRect(x1 - 4, y1 - 2, w + 8, h + 4, ILI9341_GREEN);
    }
    tft.setCursor(tabX, 10);
    tft.print(tabs[i]);
    tabX += 60;
  }

  tft.drawFastHLine(0, 35, tft.width(), ILI9341_GREEN);
  const int imgY = 45;
  const int previewH = 130;
  tft.drawRect(20, imgY, tft.width() - 40, previewH, ILI9341_GREEN);

  tft.setCursor(30, imgY + 10);
  if (selectedItem == 0) {
    float tempC = bmp.readTemperature();
    tft.setTextSize(2);
    tft.print("Temp: ");
    tft.print(tempC, 1);
    tft.print(" C");
  }
  else if (selectedItem == 1) {
    long d = readDistance();
    tft.setTextSize(2);
    tft.print("Distance: ");
    if (d < 0) tft.print("--"); else tft.print(d), tft.print(" cm");
  }
  else {
    tft.setTextSize(1);
    tft.print("Sensor preview/data here");
  }

  // Desenha pontos nos cantos do ecrã
  tft.fillRect(0, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width() - 10, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(0, tft.height() - 10, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width() - 10, tft.height() - 10, 10, 10, ILI9341_GREEN);
}








  