#include "PZEMMonitor.h"
#include "Axsecure.h"
// #include <SPIFFS.h>

PZEMMonitor::PZEMMonitor(int rxPin, int txPin, HardwareSerial& serial, LCDDisplay& lcdDisplay) 
    : lcd(lcdDisplay) {
    for(int i = 0; i < NUM_PZEMS; i++) {
        pzems[i] = PZEM004Tv30(serial, rxPin, txPin, 0x50 + i);
    }
}
void PZEMMonitor::logValues() {
    // Open file in append mode
    File file = SPIFFS.open("/pzem_readings.txt", "a");
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }

    // Get the current timestamp
    unsigned long currentTime = millis();
    
    // Log the timestamp
    file.printf("{\"timestamp\": %lu,\n", currentTime);

    // Log readings for all PZEM modules
    file.printf("\"readings\": [\n");
    for (int i = 0; i < NUM_PZEMS; i++) {
        float voltage = pzems[i].voltage();
        float current = pzems[i].current();
        float power = pzems[i].power();
        float energy = pzems[i].energy();
        float frequency = pzems[i].frequency();
        float powerFactor = pzems[i].pf();

        // Check if all readings are valid (not NaN)
        if (!isnan(voltage) && !isnan(current) && !isnan(power) && !isnan(energy) && !isnan(frequency) && !isnan(powerFactor)) {
            // Log readings for the current module
            file.printf("{\"module\": %d, \"voltage\": %.2f, \"current\": %.2f, \"power\": %.2f, \"energy\": %.2f, \"frequency\": %.2f, \"powerFactor\": %.2f}", 
                i, voltage, current, power, energy, frequency, powerFactor);
            
            // Add comma for all readings except the last one
            if (i < NUM_PZEMS - 1) {
                file.print(",\n");
            } else {
                file.print("\n");
            }
        }
    }
    file.printf("]}\n");

    // Close the file
    file.close();
}

void PZEMMonitor::readAndPrintValues() {
    static unsigned long lastLogTime = 0;

    // Arrays to store readings for all modules
    float voltages[NUM_PZEMS];
    float currents[NUM_PZEMS];
    float powers[NUM_PZEMS];
    float energies[NUM_PZEMS];
    float frequencies[NUM_PZEMS];
    float powerFactors[NUM_PZEMS];
    bool anyModuleWorking = false; // Flag to track if any module is working

    // Loop through all PZEM modules to check for readings
    for (int i = 0; i < NUM_PZEMS; i++) {
        float voltage = pzems[i].voltage();
        float current = pzems[i].current();
        float power = pzems[i].power();
        float energy = pzems[i].energy();
        float frequency = pzems[i].frequency();
        float powerFactor = pzems[i].pf();

        // If any value is not a number (NaN), set the flag to true
        if (!isnan(voltage) && !isnan(current) && !isnan(power) && !isnan(energy) && !isnan(frequency) && !isnan(powerFactor)) {
            anyModuleWorking = true;
        }

        // Store the readings
        voltages[i] = round(voltage * 10) / 10.0;
        currents[i] = round(current * 10) / 10.0;
        powers[i] = round(power * 10) / 10.0;
        energies[i] = round(energy * 10) / 10.0;
        frequencies[i] = round(frequency * 10) / 10.0;
        powerFactors[i] = round(powerFactor * 10) / 10.0;
    }

    // Only log data if any module has valid readings

    // Print readings to Serial and LCD
    for (int j = 0; j < 6; j++) {
        String attributeLine;
        String title;
        for (int i = 0; i < NUM_PZEMS; i++) {
            // Select the attribute based on the current iteration
            float attributeValue;
            switch (j) {
                case 0:
                    attributeValue = voltages[i];
                    title = "V";
                    break;
                case 1:
                    attributeValue = currents[i];
                    title = "I";
                    break;
                case 2:
                    attributeValue = powers[i];
                    title = "W";
                    break;
                case 3:
                    attributeValue = energies[i];
                    title = "Kwh";
                    break;
                case 4:
                    attributeValue = frequencies[i];
                    title = "Hz";
                    break;
                case 5:
                    attributeValue = powerFactors[i];
                    title = "PF";
                    break;
                default:
                    break;
            }

            // If the attribute value is available, add it to the line; otherwise, add 'NaN'
            if (!isnan(attributeValue)) {
                attributeLine = String(attributeValue) + " "+attributeLine ;
            } else {
                attributeLine = "NaNa "+attributeLine;
            }
        }
        lcd.displayTitle(title);
        // Display the attribute line on the LCD
        lcd.displayReadings(attributeLine);
        delay(1000); // Wait for 2 seconds before toggling to the next attribute
    }
}
void PZEMMonitor::sendDataAndClear() {
    // Check if SPIFFS is initialized
    if (!SPIFFS.begin()) {
        Serial.println("Failed to initialize SPIFFS");
        return;
    }

    // Check if the file exists
    if (!SPIFFS.exists("/pzem_readings.txt")) {
        Serial.println("File not found");
        return;
    }

    // Read the file
    File file = SPIFFS.open("/pzem_readings.txt", "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    // Read the content of the file
    String fileContent;
    while (file.available()) {
        fileContent += char(file.read());
    }
    file.close();

    // Check if the file content is empty
    if (fileContent.isEmpty()) {
        Serial.println("File content is empty");
        return;
    }

    // Replace special characters in the file content
    fileContent.replace("\n", "\\n");
    fileContent.replace("\"", "\\\"");

    // Create JSON payload with escaped characters
    // String jsonPayload = "{\"data\":\"" + fileContent + "\"}";
    String payload = "{\"deviceId\":\"1\",\"data\":\"" + fileContent + "\"}";
    // String payload = "{\"deviceId\":\"" + deviceId + "\",\"data\":\"" + fileContent + "\"}";



    // Create an instance of the Axsecure class
    Axsecure axsecure("http://192.168.137.21", 8000);

    // Send the data to the server
    Serial.println(payload); // Print the JSON payload for debugging
    bool success = axsecure.sendData(payload, "receive_data");

    if (success) {
        // Clear the file if the data was successfully sent
        if (SPIFFS.remove("/pzem_readings.txt")) {
            Serial.println("File cleared successfully");
        } else {
            Serial.println("Failed to clear file");
        }
    } else {
        Serial.println("Failed to send data to the server");
    }
}