
#include "Functions.h"
#include <Adafruit_ILI9341.h>  // Needed to access display stuff


extern Adafruit_ILI9341 tft;
#define TRIG_PIN 2
#define ECHO_PIN 3

const char* menuItems[] = { "TEMPERATURE", "DISTANCE", "GAS SENSOR", "LIGHT SENSOR" };
const int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);


void drawMainMenu(int selectedItem) {
  tft.fillScreen(ILI9341_BLACK);

  const char* tabs[] = { "TEMP", "DIS", "GAS", "LIGHT" };
  const int numTabs = sizeof(tabs) / sizeof(tabs[0]);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);

  int tabX = 15;
  for (int i = 0; i < numTabs; i++) {
    if (i == selectedItem) {
      int16_t x1, y1;
      uint16_t w, h;
      tft.getTextBounds(tabs[i], tabX, 10, &x1, &y1, &w, &h);
      tft.drawRect(x1 - 4, y1 - 2, w + 8, h + 4, ILI9341_GREEN);
    }

    tft.setCursor(tabX, 10);
    tft.print(tabs[i]);
    tabX += 60;
  }

  tft.drawFastHLine(0, 35, tft.width(), ILI9341_GREEN);

  int imgY = 45;
  int previewHeight = 130;
  tft.drawRect(20, imgY, tft.width() - 40, previewHeight, ILI9341_GREEN);
  tft.setCursor(30, imgY + 10);
  tft.setTextSize(2);

  // Show sensor data depending on tab
  if (selectedItem == 1) { // Index 1 is "DISTANCE"
    tft.print("Distance: ");
    tft.print(" ");
    tft.println(" cm");
  } else {
    tft.setTextSize(1);
    tft.print("Sensor preview/data here");
  }

  // UI border dots
  tft.fillRect(0, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width() - 10, 0, 10, 10, ILI9341_GREEN);
  tft.fillRect(0, tft.height() - 10, 10, 10, ILI9341_GREEN);
  tft.fillRect(tft.width() - 10, tft.height() - 10, 10, 10, ILI9341_GREEN);
  drawLevelInfo();
}
void addXP(int amount) {
  playerXP += amount;
  if (playerXP >= xpToNextLevel) {
    playerXP -= xpToNextLevel;
    playerLevel++;
    xpToNextLevel += 50; // or your leveling formula

    // Redraw full bar and level text on level up
    drawLevelBarOutline(playerLevel);
  }
  // Update only the fill part after XP changes
  updateXPBarFill(playerXP, xpToNextLevel);
}

void drawLevelInfo() {
  int padding = 10;                // Padding from edges
  int y = tft.height() - 30;      // Vertical position near bottom
  int barWidth = tft.width() - 2 * padding;  // Full width minus padding
  int barHeight = 10;

  // Clear previous level info area
  tft.fillRect(0, y - 2, tft.width(), barHeight + 20, ILI9341_BLACK);

  // Draw Level text on left side
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(padding, y);
  tft.printf("Level: %d", playerLevel);

  // Calculate fill width proportionally
  float fillRatio = (float)playerXP / xpToNextLevel;
  if (fillRatio > 1.0) fillRatio = 1.0;

  int fillWidth = (int)(barWidth * fillRatio);

  // Draw XP bar outline (full width)
  tft.drawRect(padding, y + 12, barWidth, barHeight, ILI9341_GREEN);

  // Draw XP bar fill
  tft.fillRect(padding + 1, y + 13, fillWidth, barHeight - 2, ILI9341_GREEN);
}
void drawLevelBarOutline(int level) {
  int padding = 10;
  int y = tft.height() - 30;
  int barWidth = tft.width() - 2 * padding;
  int barHeight = 10;

  // Clear the level bar area (text + bar)
  tft.fillRect(0, y - 2, tft.width(), barHeight + 20, ILI9341_BLACK);

  // Draw Level text
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(padding, y);
  tft.printf("Level: %d", level);

  // Draw XP bar outline
  tft.drawRect(padding, y + 12, barWidth, barHeight, ILI9341_GREEN);
}

void updateXPBarFill(int currentXP, int maxXP) {
  int padding = 10;
  int y = tft.height() - 30;
  int barWidth = tft.width() - 2 * padding;
  int barHeight = 10;

  float fillRatio = (float)currentXP / maxXP;
  if (fillRatio > 1.0) fillRatio = 1.0;
  int fillWidth = (int)(barWidth * fillRatio);

  // Clear previous filled area first (inside bar)
  tft.fillRect(padding + 1, y + 13, barWidth - 2, barHeight - 2, ILI9341_BLACK);

  // Draw new filled part
  if (fillWidth > 0) {
    tft.fillRect(padding + 1, y + 13, fillWidth, barHeight - 2, ILI9341_GREEN);
  }
}
  