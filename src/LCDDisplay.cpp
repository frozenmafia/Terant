#include "LCDDisplay.h"

LCDDisplay::LCDDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : lcd(lcd_addr, lcd_cols, lcd_rows) {}

void LCDDisplay::begin() {
    lcd.init();
    lcd.backlight();
    createCustomCharacters();
    lcd.clear();
}

void LCDDisplay::displayText(const String &text, uint8_t row, uint8_t col) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

void LCDDisplay::displayWiFiStatus(bool isConnected) {
    lcd.setCursor(15, 0); // Top right corner
    if (isConnected) {
        lcd.write('C'); // WiFi connected symbol
    } else {
        lcd.write('N'); // WiFi disconnected symbol
    }
}

void LCDDisplay::displayLoading(bool isButtonPressed) {
    lcd.setCursor(14, 0);
    if (isButtonPressed) {
        lcd.write(byte(2)); // Display loading symbol
    } else {
        lcd.print(" "); // Clear the loading symbol
    }
}

void LCDDisplay::displayReset(){
    lcd.setCursor(15,0);
    lcd.write('R');
}

void LCDDisplay::displayTitle(const String &title) {
    // Clear the area where the title will be displayed
    lcd.setCursor(0, 0); // Set cursor to the starting position
    for (uint8_t i = 0; i < 5; i++) {
        lcd.print(" "); // Print spaces to clear the area
    }

    // Print the title
    lcd.setCursor(0, 0); // Set cursor back to the starting position
    lcd.print(title);    // Print the title
}

void LCDDisplay::displayReadings(const String &readings) {
    // Extract the first reading and display it at position (0, 6)
    String firstReading = readings.substring(0, 9);
    lcd.setCursor(4, 0);
    lcd.print(firstReading);

    // Extract the remaining readings and display them on the second row
    String remainingReadings = readings.substring(9);
    lcd.setCursor(0, 1); // Move to position (5, 1) for the second reading
    for (uint8_t i = 0; i < 16; i++) {
        lcd.print(" "); // Print spaces to clear the area
    }
    lcd.setCursor(0, 1);
    lcd.print(remainingReadings);
}


void LCDDisplay::createCustomCharacters() {
    lcd.createChar(0, wifiConnectedChar);
    lcd.createChar(1, wifiDisconnectedChar);
    lcd.createChar(2, loadingSymbol); // Create the loading symbol character
}

void LCDDisplay::clear() {
    lcd.clear();
}
