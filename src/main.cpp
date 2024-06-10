#include <Arduino.h>
#include <TimeLib.h> // Include TimeLib library for time calculations
#include "ButtonHandler.h"
#include "WifiConfigurator.h"
#include "PZEMMonitor.h"
#include <SPIFFS.h>
#include "LCDDisplay.h"
#include <Axsecure.h>

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

void handleESP32(void *pvParameters) {
    for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
            if (wifiConfigurator.retry()) {
                    pzemMonitor.logValues();
                    delay(100);
                    pzemMonitor.sendDataAndClear();
            }
            delay(2000); // Short delay to show the connection status
        }

        if (buttonHandler.isButtonPressed()) {
            if (wifiConfigurator.connect(false)) {
                    pzemMonitor.logValues();
                    delay(100);
                    pzemMonitor.sendDataAndClear();
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to yield control
    }
}

void displayReadings(void *pvParameters) {
    unsigned long lastLogTime = 0;
    const unsigned long LogInterval = 1; // 1 hour in seconds
    const int MaxLogsBeforeSend = 1;
    int logCount = 0;

    for (;;) {
        lcdDisplay.displayWiFiStatus(WiFi.status() == WL_CONNECTED);
        time_t currentTime = now(); // Get current time using TimeLib

        pzemMonitor.readAndPrintValues();

        if (currentTime - lastLogTime >= LogInterval) {
            lastLogTime = currentTime;
            pzemMonitor.logValues();
            logCount++;
            if (logCount >= MaxLogsBeforeSend) {
                pzemMonitor.sendDataAndClear();
                logCount = 0;
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to yield control
    }
}

void setup() {
    Serial.begin(115200);
    lcdDisplay.begin();
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to initialize SPIFFS");
        return;
    } else {
        Serial.println("Initialized");
    }
    lcdDisplay.displayText("Terant...", 0, 0);
    wifiConfigurator.begin();

    if (WiFi.status() == WL_CONNECTED) {

        pzemMonitor.logValues();
        delay(100);
        pzemMonitor.sendDataAndClear();
    }

    xTaskCreatePinnedToCore(
        handleESP32,    /* Task function */
        "HandleESP32",  /* Name of the task */
        10000,          /* Stack size in words */
        NULL,           /* Task input parameter */
        1,              /* Priority of the task */
        NULL,           /* Task handle */
        0);             /* Core where the task should run */

    xTaskCreatePinnedToCore(
        displayReadings,    /* Task function */
        "DisplayReadings",  /* Name of the task */
        10000,              /* Stack size in words */
        NULL,               /* Task input parameter */
        1,                  /* Priority of the task */
        NULL,               /* Task handle */
        1);                 /* Core where the task should run */
}

void loop() {
    // Empty. Tasks are running in parallel.
}
