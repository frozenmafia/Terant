#include "WifiConfigurator.h"

void WifiConfigurator::begin() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(3000);
  Serial.println("\n Starting");
}

bool WifiConfigurator::connect() {
  WiFiManager wm;
  bool res = wm.autoConnect("AutoConnectAP","password");
  if(!res) {
    Serial.println("Failed to connect or hit timeout");
  } 
  else {
    Serial.println("Connected to WiFi!");
  }
  return res;
}
