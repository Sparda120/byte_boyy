#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Uses User_Setup.h or custom setup

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3); // Or 3, depending on your screen
  tft.invertDisplay(1);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("Hello from TFT_eSPI");
}

void loop() {
  // Empty or small animation test
}