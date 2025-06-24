#include "Functions.h"
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>

extern Adafruit_ILI9341 tft;
extern const int trigPin;
extern const int echoPin;
extern Adafruit_BMP280 bmp;

// ——— Sensor helper with debug ———
// Returns distance in cm, or -1 if out of range.
// Also prints the raw pulseIn() duration (µs) to Serial.
long readDistance() {
  // Make sure trigger is LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(500);

  // Send 10 µs HIGH pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin, LOW);

  // Wait for echo (timeout 40 ms)
  unsigned long duration = pulseIn(echoPin, HIGH, 40000UL);
  Serial.print("raw pulseIn: ");
  Serial.print(duration);
  Serial.println(" µs");

  if (duration == 0) {
    return -1;    // no echo within timeout
  }
  // 58 µs per cm
  return duration / 58;
}

// ——— Draw Distance tab frame once ———
void drawDistanceScreen() {
  tft.fillScreen(ILI9341_BLACK);

  // Tab bar (highlight "DIS")
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

  // Data box
  int imgY = 45;
  tft.drawRect(20, imgY, tft.width() - 40, 130, ILI9341_GREEN);

  // Static label
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(30, imgY + 10);
  tft.print("Distance:");

  // Corner dots
  tft.fillRect(0, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width()-10, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(0, tft.height()-10, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width()-10, tft.height()-10, 10, 10, ILI9341_GREEN);
}

// ——— Only overwrite the numeric value ———
void updateDistanceValue() {
  long d = readDistance();

  // Clear old reading (tweak w/h if needed)
  int x = 140, y = 55;
  tft.fillRect(x, y, 80, 16, ILI9341_BLACK);

  // Print new
  tft.setCursor(x, y);
  if (d < 0) {
    tft.print("--");
  } else {
    tft.print(d);
    tft.print(" cm");
  }
}

// —— Fallback full-screen menu for non-Distance tabs —— 
void drawMainMenu(int selectedItem) {
  // 1) Clear
  tft.fillScreen(ILI9341_BLACK);

  // 2) Draw tabs
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

  // 3) Separator and box
  tft.drawFastHLine(0, 35, tft.width(), ILI9341_GREEN);
  const int imgY = 45;
  const int previewH = 130;
  tft.drawRect(20, imgY, tft.width() - 40, previewH, ILI9341_GREEN);

  // 4) Sensor data
  tft.setCursor(30, imgY + 10);
  if (selectedItem == 0) {
    // TEMPERATURE
    float tempC = bmp.readTemperature();
    tft.setTextSize(2);
    tft.print("Temp: ");
    tft.print(tempC, 1);
    tft.print(" C");
  }
  else if (selectedItem == 1) {
    // DISTANCE
    long d = readDistance();
    tft.setTextSize(2);
    tft.print("Distance: ");
    if (d < 0) {
      tft.print("--");
    } else {
      tft.print(d);
      tft.print(" cm");
    }
  }
  else {
    // GAS or LIGHT
    tft.setTextSize(1);
    tft.print("Sensor preview/data here");
  }

  // 5) UI border dots
  tft.fillRect(0, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width() - 10, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(0, tft.height() - 10, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width() - 10, tft.height() - 10, 10, 10, ILI9341_GREEN);
}  // <-- this closing brace matches drawMainMenu(










  