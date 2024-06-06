#include <Arduino.h>
#include "ButtonHandler.h"
#include "WifiConfigurator.h"
#include "PZEMMonitor.h"
#include <SPIFFS.h>
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

void handleESP32(void *pvParameters) {
    for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
            // lcdDisplay.displayWiFiStatus(false);
            if(wifiConfigurator.retry()){
                pzemMonitor.sendDataAndClear();
            }
            delay(2000); // Short delay to show the connection status
        }

        if(buttonHandler.isButtonPressed()){
            if(wifiConfigurator.connect(false)){
                pzemMonitor.sendDataAndClear();
            }

        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to yield control
    }
}

void displayReadings(void *pvParameters) {
    unsigned long lastLogTime = 0;
    unsigned long lastReadTime = 0;
    const unsigned long LogInterval = 10000; // 10 seconds
    const unsigned long ReadInterval = 2000; // 2 seconds
    const int MaxLogsBeforeSend = 12; // Maximum logs before sending to the server
    int logCount = 0;
    int readCount = 0;


    for (;;) {
        lcdDisplay.displayWiFiStatus(WiFi.status() == WL_CONNECTED);
        unsigned long currentMillis = millis();

        // Check if it's time to log data
        if (currentMillis - lastLogTime >= ReadInterval) {
            lastLogTime = currentMillis;
            // Log the PZEM values to the file
            pzemMonitor.readAndPrintValues();

            readCount++;
            if(readCount>=1800){
                pzemMonitor.logValues();
                logCount++;
                if (logCount >= MaxLogsBeforeSend) {
                    pzemMonitor.sendDataAndClear();
                    logCount = 0;
                }
            }
        }

        // Check if it's time to read and display PZEM value

        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to yield control
    }
}

void setup() {
    Serial.begin(115200);
    lcdDisplay.begin();
       if (!SPIFFS.begin(true)) {
        Serial.println("Failed to initialize SPIFFS");
        return;
    }else{
        Serial.println("Initialized");
    }
    lcdDisplay.displayText("Terant...", 0, 0);
    wifiConfigurator.begin();
    // lcdDisplay.clear();

    if (!SPIFFS.exists("/pzem_readings.txt")) {
        Serial.println("File not found");
        // return;
    }else{
        pzemMonitor.sendDataAndClear();
    }
    
    // Create tasks
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
