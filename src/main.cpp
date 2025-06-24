#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_BMP280.h>

#include "Functions.h"        // Declara métodos e variáveis externas

#include <WiFi.h>
#include <HTTPClient.h>

// --- Pinos de hardware ---
const int buzzerPin   = 8;    // Buzzer para som de boot
const int trigPin     = 3;    // Trigger do sensor ultrassónico
const int echoPin     = 2;    // Echo do sensor ultrassónico
const int button1Pin  = 7;    // Botão para seleccionar tab DISTÂNCIA
const int button2Pin  = 4;    // Botão para seleccionar tab TEMPERATURA

// --- Wi-Fi & ThingSpeak ---
const char* ssid     = "Samsung S10";
const char* password = "jlopes22104";
const char* server   = "http://api.thingspeak.com/update";
const String apiKey  = "TD8XH1C81H13GBUV";

// --- Estado global ---
int selectedMenuItem = 1;    // Tab inicial: DISTÂNCIA

// Objetos globais definidos em main (ligados ao extern em Functions.h)
Adafruit_ILI9341 tft(10, 5, 6);
Adafruit_BMP280  bmp;

// Função de configuração: corre uma vez ao iniciar o microcontrolador
void setup() {
  Serial.begin(115200);              // Inicia a porta série para debug

  // Inicialização do sensor BMP280 via I2C
  Wire.begin();  
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 init failed!");
  } else {
    Serial.println("BMP280 online");
  }

  // Inicialização do display TFT
  tft.begin();
  tft.setRotation(1);
  tft.invertDisplay(true);

  // Configuração dos modos dos pinos
  pinMode(buzzerPin,   OUTPUT);
  pinMode(trigPin,     OUTPUT);
  pinMode(echoPin,     INPUT);
  pinMode(button1Pin,  INPUT_PULLUP);
  pinMode(button2Pin,  INPUT_PULLUP);

  // Sequência de boot no ecrã
  drawRandomHex(20);
  wipeTransition(ILI9341_BLACK);
  bootCheckSequence();
  wipeTransition(ILI9341_BLACK);
  showWelcome();
  wipeTransition(ILI9341_BLACK);

  // Desenho inicial do menu conforme tab selecionada
  if (selectedMenuItem == 1) {
    drawDistanceScreen();
  } else {
    drawMainMenu(selectedMenuItem);
  }

  // Ligação à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");
}

// Função principal ciclica: corre continuamente após setup()
void loop() {
  // 1) Leitura imediata dos botões para mudar tab
  if (digitalRead(button1Pin) == LOW) {
    playClickSound(); 
    selectedMenuItem = 1;            // Seleciona DISTÂNCIA
    while (digitalRead(button1Pin) == LOW) {}  // Esmaga debouncing
    delay(50);
  }
  if (digitalRead(button2Pin) == LOW) {
    playClickSound(); 
    selectedMenuItem = 0;            // Seleciona TEMPERATURA
    while (digitalRead(button2Pin) == LOW) {}
    delay(50);
  }

  // 2) Redesenho do ecrã se a tab mudou
  static int lastMenu = -1;
  if (selectedMenuItem != lastMenu) {
    playClickSound(); 
    if (selectedMenuItem == 1) drawDistanceScreen();
    else                      drawMainMenu(selectedMenuItem);
    lastMenu = selectedMenuItem;
  }

  // 3) Atualização periódica do valor no ecrã (500 ms)
  static unsigned long lastDisplayUpdate = 0;
  const unsigned long displayInterval = 500;
  unsigned long now = millis();

  if (now - lastDisplayUpdate >= displayInterval) {
    if (selectedMenuItem == 1) updateDistanceValue();
    lastDisplayUpdate = now;
  }

  // 4) Envio de dados para ThingSpeak a cada 15 s
  static unsigned long lastThingSpeak = 0;
  const unsigned long tsInterval = 15000;

  if (now - lastThingSpeak >= tsInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      float temp = bmp.readTemperature();             // Lê temperatura
      long rawDist = readDistance();                  // Lê distância
      float distVal = (rawDist < 0 ? 0.0f : rawDist);

      HTTPClient http;
      String url = String(server)
        + "?api_key=" + apiKey
        + "&field1=" + String(temp, 1)
        + "&field2=" + String(distVal, 1);

      http.begin(url);
      int code = http.GET();                         // Requisição HTTP
      if (code > 0) Serial.printf("ThingSpeak OK, code=%d\n", code);
      else          Serial.printf("ThingSpeak ERR, code=%d\n", code);
      http.end();
    } else {
      Serial.println("WiFi desconectado.");
    }
    lastThingSpeak = now;
  }
}
  

