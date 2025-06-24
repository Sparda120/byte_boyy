#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_BMP280.h>

#include "Functions.h"        // declares extern tft, extern bmp, readDistance(), drawDistanceScreen(), updateDistanceValue(), drawMainMenu()

#include <WiFi.h>
#include <HTTPClient.h>

// Pin assignments
const int buzzerPin   = 8;
const int trigPin     = 3;
const int echoPin     = 2;
const int button1Pin  = 7;   // Go to Distance
const int button2Pin  = 4;   // Go to Temperature (or Gas)

// Wi-Fi & ThingSpeak
const char* ssid     = "Samsung S10";
const char* password = "jlopes22104";
const char* server   = "http://api.thingspeak.com/update";
const String apiKey  = "TD8XH1C81H13GBUV";

// State
int selectedMenuItem = 1;    // start on Distance

// Global objects (match “extern” in Functions.h)
Adafruit_ILI9341 tft(10, 5, 6);
Adafruit_BMP280  bmp;

void setup() {
  Serial.begin(115200);

  // — I2C & BMP280 init —
  Wire.begin();  
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 init failed!");
  } else {
    Serial.println("BMP280 online");
  }

  // — Display init —
  tft.begin();
  tft.setRotation(1);
  tft.invertDisplay(true);

  // — Pins —
  pinMode(buzzerPin,   OUTPUT);
  pinMode(trigPin,     OUTPUT);
  pinMode(echoPin,     INPUT);
  pinMode(button1Pin,  INPUT_PULLUP);
  pinMode(button2Pin,  INPUT_PULLUP);

  // — Boot sequence —
  drawRandomHex(20);
  wipeTransition(ILI9341_BLACK);
  bootCheckSequence();
  wipeTransition(ILI9341_BLACK);
  showWelcome();
  wipeTransition(ILI9341_BLACK);

  // — Initial menu draw —
  if (selectedMenuItem == 1) {
    drawDistanceScreen();
  } else {
    drawMainMenu(selectedMenuItem);
  }

  // — Connect Wi-Fi once —
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");
}

  void loop() {
  // — 1) Handle button presses immediately —
  if (digitalRead(button1Pin) == LOW) {
    selectedMenuItem = 1;            // Distance tab
    while (digitalRead(button1Pin) == LOW) {}  // wait release
    delay(50);                       // tiny debounce
  }
  if (digitalRead(button2Pin) == LOW) {
    selectedMenuItem = 0;            // Temperature tab
    while (digitalRead(button2Pin) == LOW) {}
    delay(50);
  }

  // — 2) Tab‐change detection & initial draw —
  static int  lastMenu    = -1;
  if (selectedMenuItem != lastMenu) {
    if (selectedMenuItem == 1) {
      drawDistanceScreen();
    } else {
      drawMainMenu(selectedMenuItem);
    }
    lastMenu = selectedMenuItem;
    // reset the display‐update timer so it fires right away next
    // (we’ll set lastDisplayUpdate below)
  }

  // — 3) Nonblocking display updates (every 500 ms) —
  static unsigned long lastDisplayUpdate = 0;
  const unsigned long displayInterval = 500;
  unsigned long now = millis();

  if (now - lastDisplayUpdate >= displayInterval) {
    // Only update the numeric reading if on Distance tab
    if (selectedMenuItem == 1) {
      updateDistanceValue();
    }
    lastDisplayUpdate = now;
  }

  // — 4) Nonblocking ThingSpeak send (every 15 s) —
  static unsigned long lastThingSpeak = 0;
  const unsigned long tsInterval = 15000;

  if (now - lastThingSpeak >= tsInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      // Read actual sensors
      float valorField1 = bmp.readTemperature();       // °C
      long rawDist      = readDistance();              // cm or -1
      float valorField2 = (rawDist < 0 ? 0.0f : rawDist);

      HTTPClient http;
      String url = String(server)
        + "?api_key=" + apiKey
        + "&field1=" + String(valorField1, 1)
        + "&field2=" + String(valorField2, 1);

      http.begin(url);
      int code = http.GET();
      if (code > 0) {
        Serial.printf("ThingSpeak OK, code=%d\n", code);
      } else {
        Serial.printf("ThingSpeak ERR, code=%d\n", code);
      }
      http.end();
    } else {
      Serial.println("WiFi desconectado.");
    }
    lastThingSpeak = now;
  }

  // — 5) Return immediately — no long delays here —
}



  

