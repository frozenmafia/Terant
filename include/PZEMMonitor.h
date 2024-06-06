#ifndef PZEM_MONITOR_H
#define PZEM_MONITOR_H

#include <PZEM004Tv30.h>
#include "LCDDisplay.h"

#define NUM_PZEMS 5

class PZEMMonitor {
public:
    PZEMMonitor(int rxPin, int txPin, HardwareSerial& serial, LCDDisplay& lcd);
    void readAndPrintValues();

private:
    PZEM004Tv30 pzems[NUM_PZEMS];
    LCDDisplay& lcd;
};

#endif
