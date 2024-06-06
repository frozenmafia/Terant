#ifndef PZEMMONITOR_H
#define PZEMMONITOR_H

#include <PZEM004Tv30.h>
#include <HardwareSerial.h>
#include <LCDDisplay.h>
#include <LittleFS.h>
#include <SPIFFS.h>

#define NUM_PZEMS 5
#define LOG_INTERVAL 600000 // 10 minutes in milliseconds
#define MAX_FILE_SIZE 2097152 // 2 MB

class PZEMMonitor {
public:
    PZEMMonitor(int rxPin, int txPin, HardwareSerial& serial, LCDDisplay& lcdDisplay);
    void readAndPrintValues();
    void logValues();
    void sendDataAndClear();

private:
    PZEM004Tv30 pzems[NUM_PZEMS];
    LCDDisplay& lcd;
};

#endif // PZEMMONITOR_H
