#ifndef PZEM_MONITOR_H
#define PZEM_MONITOR_H

#include <PZEM004Tv30.h>

class PZEMMonitor {
public:
  PZEMMonitor(int rxPin, int txPin, HardwareSerial& serial);
  void readAndPrintValues();

private:
  PZEM004Tv30 pzem;
};

#endif
