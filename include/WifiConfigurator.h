#ifndef WIFI_CONFIGURATOR_H
#define WIFI_CONFIGURATOR_H

#include <WiFiManager.h>

class WifiConfigurator {
public:
  void begin();
  bool connect();
};

#endif
