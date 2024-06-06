#include <Arduino.h>
#include "ButtonHandler.h"
#include "WifiConfigurator.h"
#include "PZEMMonitor.h"
#include "LCDDisplay.h"

#define TRIGGER_PIN 0
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

ButtonHandler buttonHandler(TRIGGER_PIN);
LCDDisplay lcdDisplay(LCD_ADDR, LCD_COLS, LCD_ROWS);
WifiConfigurator wifiConfigurator(lcdDisplay);
PZEMMonitor pzemMonitor(PZEM_RX_PIN, PZEM_TX_PIN, Serial2, lcdDisplay);

void setup() {
    Serial.begin(115200);
    lcdDisplay.begin();
    lcdDisplay.displayText("Starting...", 0, 0);
    // delay(2000);
    wifiConfigurator.begin();

    
    
    lcdDisplay.clear();
}

void loop() {
    static unsigned long lastPZEMReadTime = 0;
    const unsigned long PZEMReadInterval = 2000; // 2 seconds

    if (buttonHandler.isButtonPressed()) {
      lcdDisplay.displayLoading(true);
      Serial.println("Button Pressed");
        delay(300); // Short delay to debounce the button

        // if (buttonHandler.isButtonPressed()) {
        //     // lcdDisplay.displayText("", 0, 0);
        //     Serial.println("Erasing Config, restarting");
        //     lcdDisplay.displayReset();
        //     // lcdDisplay.displayText("Erasing Config,", 0, 0);
        //     // lcdDisplay.displayText("restarting", 1, 0);
        //     WiFiManager().resetSettings();
        //     ESP.restart();
        // } else {
            Serial.println("Starting config portal");
            // lcdDisplay.displayText("Starting config", 0, 0);
            // lcdDisplay.displayText("portal", 1, 0);
            
            if (wifiConfigurator.connect()) {
                // lcdDisplay.displayWiFiStatus(true);
                    lcdDisplay.displayLoading(false);
            } else {
              // lcdDisplay.displayWiFiStatus(false);
                  lcdDisplay.displayLoading(false);
            }
            lcdDisplay.displayLoading(false);
            delay(2000); // Short delay to show the connection status
        // }
    

    }
    unsigned long currentMillis = millis();

    lcdDisplay.displayWiFiStatus(WiFi.status() == WL_CONNECTED);
    if (currentMillis - lastPZEMReadTime >= PZEMReadInterval) {
        lastPZEMReadTime = currentMillis;
        // Read PZEM values and print them on Serial and LCD
        pzemMonitor.readAndPrintValues();
    }
}
