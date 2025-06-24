#include "Functions.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

int cornerSize = 20;
int thickness = 3;  // Largura dos cantos
uint16_t color = ILI9341_GREEN;

extern Adafruit_ILI9341 tft;

const char* checks[] = {
  "[OK] Bootloader v3.2 initializing...",
  "[OK] Vault-Tec Protocols loaded.",
  "[OK] Memory banks: 8192KB detected.",
  "[OK] BIOS integrity verified.",
  "[OK] Byte-Boy sensor array online.",
  "[OK] Personal data link established.",
  "[OK] Time sync: STABLE",
  "[OK] Environment scan: CLEAN",
  "[OK] Audio output enabled.",
  "[OK] Display buffer cleared.",
  "[OK] User profile: ACTIVE",
  "[OK] Power cell: 78% capacity",
  "[OK] Encryption module: ENABLED",
  "[OK] Geolocation lock: ACQUIRED",
  "[OK] Radio receiver: CHANNEL 5 READY",
  "[OK] Thermal sensor array: STABLE"
};

// Desenha linhas aleatórias em formato hexadecimal no ecrã
void drawRandomHex(int lines) {
  tft.setRotation(3);
  tft.invertDisplay(1);
  tft.setTextColor(color, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.fillScreen(ILI9341_BLACK);

  for (int i = 0; i < lines; i++) {
    char hexLine[50] = {0};
    for (int j = 0; j < 16; j++) {
      byte val = random(0, 256);
      char buf[4];
      if (val < 16) sprintf(buf, "0%X ", val);
      else           sprintf(buf, "%X ", val);
      strcat(hexLine, buf);
    }
    tft.setCursor(0, i * 10);
    tft.print(hexLine);
    delay(100);
  }
}

// Executa sequência de verificações iniciais mostradas no ecrã
void bootCheckSequence() {
  tft.setRotation(3);
  tft.invertDisplay(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(color);
  tft.setTextSize(1);
  int y = 0;

  int count = sizeof(checks) / sizeof(checks[0]);
  for (int i = 0; i < count; i++) {
    tft.setCursor(0, y);
    tft.println(checks[i]);
    y += 12;

    // Delay maior nos últimos 3 checks
    if (i < count - 3) delay(250);
    else               delay(800);
  }
}

// Mostra uma mensagem de boas-vindas com decorações nos cantos
void showWelcome() {
  delay(800);
  tft.setRotation(3);
  tft.invertDisplay(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(color);
  
  // Desenha cantos decorativos
  // Canto superior esquerdo
  tft.fillRect(0, 0, cornerSize, thickness, color);
  tft.fillRect(0, 0, thickness, cornerSize, color);
  // Canto superior direito
  tft.fillRect(tft.width() - cornerSize, 0, cornerSize, thickness, color);
  tft.fillRect(tft.width() - thickness, 0, thickness, cornerSize, color);
  // Canto inferior esquerdo
  tft.fillRect(0, tft.height() - thickness, cornerSize, thickness, color);
  tft.fillRect(0, tft.height() - cornerSize, thickness, cornerSize, color);
  // Canto inferior direito
  tft.fillRect(tft.width() - cornerSize, tft.height() - thickness, cornerSize, thickness, color);
  tft.fillRect(tft.width() - thickness, tft.height() - cornerSize, thickness, cornerSize, color);

  // Texto de boas-vindas e informações
  tft.setCursor(20, 20);
  tft.println("Welcome, User");
  tft.setTextSize(1);
  tft.setCursor(20, 50);   tft.println("Byte-Boy Interface v1.0");
  tft.setCursor(20, 60);   tft.println("Powered by: Arduino Nano ESP32");
  tft.setCursor(20, 80);   tft.println("Disciplina: Laboratorio de Microssistemas");
  tft.setCursor(20, 90);   tft.println("Instituto Politecnico de Tomar");
  tft.setCursor(20, 140);  tft.println("Developed by:");
  tft.setCursor(30, 150);  tft.println("Carlos Covao");
  tft.setCursor(30, 160);  tft.println("Carolina Casquilha");
  tft.setCursor(30, 170);  tft.println("Jose Lopes");
  tft.setCursor(20, 220);  tft.println("Version 0.0.1 - Beta Test");

  // Toca som de boot do Pip-Boy
  playPipBoyBootSound();
}

// Efeito de transição tipo 'wipe' preenchendo linhas no ecrã
void wipeTransition(uint16_t bgColor) {
  for (int y = 0; y < tft.height(); y++) {
    tft.drawFastHLine(0, y, tft.width(), bgColor);
    delay(5);  // velocidade da transição
  }
}

// Reproduz sequência de tons no buzzer para boot sound
void playPipBoyBootSound() {
  tone(buzzerPin, 880);  delay(150);
  tone(buzzerPin, 988);  delay(100);
  tone(buzzerPin, 1047); delay(100);
  tone(buzzerPin, 1175); delay(120);
  tone(buzzerPin, 1397); delay(150);
  tone(buzzerPin, 1760); delay(200);
  noTone(buzzerPin);     delay(100);

  tone(buzzerPin, 880);  delay(300);
  tone(buzzerPin, 440);  delay(300);
  noTone(buzzerPin);
}
