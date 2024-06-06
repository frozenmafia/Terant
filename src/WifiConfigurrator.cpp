#include "WifiConfigurator.h"

WifiConfigurator::WifiConfigurator(LCDDisplay& lcdDisplay) : lcd(lcdDisplay) {}

void WifiConfigurator::begin() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(3000);
    String msg = "\n Starting";
    Serial.println(msg);
    // lcd.displayText(msg, 0, 0);
    connect();
}

bool WifiConfigurator::connect() {
    WiFiManager wm;
    // lcd.displayText("Connecting to", 0, 0);
    // lcd.displayText("WiFi...", 1, 0);
    bool res = wm.autoConnect("AutoConnectAP", "password");
    if (!res) {
        String msg = "Failed to connect";
        Serial.println(msg);
        lcd.displayWiFiStatus(true);
        // lcd.displayText(msg, 0, 0);
    } else {
        String msg = "Connected to WiFi!";
        Serial.println(msg);
        lcd.displayWiFiStatus(false);
        // lcd.displayText(msg, 0, 0);
    }
    delay(3000); // Short delay to show the connection status
    return res;
}
