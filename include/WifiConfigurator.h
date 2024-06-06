#ifndef WIFI_CONFIGURATOR_H
#define WIFI_CONFIGURATOR_H

#include <WiFiManager.h>
#include "LCDDisplay.h"

class WifiConfigurator {
public:
    WifiConfigurator(LCDDisplay& lcd);
    void begin();
    bool connect();

private:
    LCDDisplay& lcd;
};

#endif
