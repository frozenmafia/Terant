#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Wire.h> // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h>

class LCDDisplay {
public:
    LCDDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void displayText(const String &text, uint8_t row, uint8_t col);
    void displayWiFiStatus(bool isConnected);
    void displayTitle(const String &title);
    void displayReadings(const String &readings);
    void displayLoading(bool isButtonPressed);
    void displayReset();
    void clear();

private:
    LiquidCrystal_I2C lcd;
    typedef uint8_t byte; // Define byte type
    uint8_t wifiConnectedChar[8] = {
        0b00000,
        0b00000,
        0b00100,
        0b01010,
        0b10001,
        0b00000,
        0b00100,
        0b00000
    };
    uint8_t wifiDisconnectedChar[8] = {
        0b00000,
        0b00000,
        0b00100,
        0b01010,
        0b10001,
        0b00000,
        0b00000,
        0b00000
    };
    uint8_t loadingSymbol[8] = { // Define the loading symbol character
        0b00000,
        0b00000,
        0b00000,
        0b01110,
        0b10001,
        0b10001,
        0b01110,
        0b00000
    };
    void createCustomCharacters();
};

#endif