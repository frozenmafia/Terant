#include <Arduino.h>
#include "ButtonHandler.h"
#include "WifiConfigurator.h"
#include "PZEMMonitor.h"

#define TRIGGER_PIN 0
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

ButtonHandler buttonHandler(TRIGGER_PIN);
WifiConfigurator wifiConfigurator;
PZEMMonitor pzemMonitor(PZEM_RX_PIN, PZEM_TX_PIN, Serial2);

void setup() {
  wifiConfigurator.begin();
  pzemMonitor.readAndPrintValues();
}

void loop() {
  if(buttonHandler.isButtonPressed()) {
    Serial.println("Button Pressed");
    delay(3000);
    if(buttonHandler.isButtonPressed()) {
      Serial.println("Button Held");
      Serial.println("Erasing Config, restarting");
      WiFiManager().resetSettings();
      ESP.restart();
    }
    Serial.println("Starting config portal");
    wifiConfigurator.connect();
  }
  pzemMonitor.readAndPrintValues();
  delay(2000);
}
