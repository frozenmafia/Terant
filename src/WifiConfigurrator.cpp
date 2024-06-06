#include "WifiConfigurator.h"

WifiConfigurator::WifiConfigurator(LCDDisplay& lcdDisplay) : lcd(lcdDisplay) {}

void WifiConfigurator::begin() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(3000);
    String msg = "\n Starting";
    Serial.println(msg);
    connect(true);
}

bool WifiConfigurator::connect(bool confPort) {
    WiFiManager wm;
    
    // Reset WiFiManager settings to forget any saved WiFi credentials

    if(!confPort)
        wm.resetSettings();

    // Enable configuration portal to start AP mode
    wm.setEnableConfigPortal(confPort);
    
    // Attempt to connect to the configured network
    bool res = wm.autoConnect("AutoConnectAP", "password");
    
    // Check connection result
    if (!res) {
        String msg = "Failed to connect";
        Serial.println(msg);
        lcd.displayWiFiStatus(true);
    } else {
        String msg = "Connected to WiFi!";
        Serial.println(msg);
        lcd.displayWiFiStatus(false);
    }

    delay(1000); // Short delay to show the connection status
    return res;
}


bool WifiConfigurator::retry() {
    WiFiManager wm;
    wm.setConnectTimeout(180);
    wm.setConnectRetries(100);
    if (wm.getWiFiIsSaved()) wm.setEnableConfigPortal(false);
    bool res = wm.autoConnect("AutoConnectAP", "password");
    if (!res) {
        String msg = "Failed to connect";
        Serial.println(msg);
        lcd.displayWiFiStatus(true);
        delay(1000); // Short delay to show the connection status
        return false;
    } else {
        String msg = "Connected to WiFi!";
        Serial.println(msg);
        lcd.displayWiFiStatus(false);
        delay(1000); // Short delay to show the connection status
        return true;
    }
    
}
