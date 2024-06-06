#include "PZEMMonitor.h"
#include "Axsecure.h"

PZEMMonitor::PZEMMonitor(int rxPin, int txPin, HardwareSerial& serial, LCDDisplay& lcdDisplay) 
    : lcd(lcdDisplay) {
    for(int i = 0; i < NUM_PZEMS; i++) {
        pzems[i] = PZEM004Tv30(serial, rxPin, txPin, 0x50 + i);
    }
}

// Define the constructor for PZEMMonitor
void PZEMMonitor::readAndPrintValues() {
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
        if (!isnan(voltage) || !isnan(current) || !isnan(power) || !isnan(energy) || !isnan(frequency) || !isnan(powerFactor)) {
            anyModuleWorking = true;
        }

        // Store the readings
        voltages[i] = voltage;
        currents[i] = current;
        powers[i] = power;
        energies[i] = energy;
        frequencies[i] = frequency;
        powerFactors[i] = powerFactor;
    }
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
                        title= "pF";
                        break;
                    default:
                        break;
                }

                // If the attribute value is available, add it to the line; otherwise, add 'E'
                if (!isnan(attributeValue)) {
                    attributeLine += String(attributeValue) + " ";
                } else {
                    attributeLine += "NaN ";
                }
            }
            lcd.displayTitle(title);
            // Display the attribute line on the LCD
            lcd.displayReadings(attributeLine);
            delay(2000); // Wait for 2 seconds before toggling to the next attribute
        }
    // } else {
    //     // If no module is working, display 'E' for all attributes for all modules
    //     String eLine = "E E E E E E";
    //     lcd.displayText(eLine, 1, 0);
    //     delay(2000); // Wait for 2 seconds before toggling to the next attribute
    // }

    // Display Wi-Fi status symbol on the second line
    
    // Create an instance of the Axsecure class
    // Axsecure axsecure("https://forgerrr.com", 8000); // Replace with your server's IP address or domain name and port

    // // Example of sending data to the server
    // String jsonData = "{";
    // jsonData += "\"Voltage\": \"" + String(voltage) + "\",";
    // jsonData += "\"Current\": \"" + String(current) + "\",";
    // jsonData += "\"Power\": \"" + String(power) + "\",";
    // jsonData += "\"Energy\": \"" + String(energy, 3) + "\",";
    // jsonData += "\"Frequency\": \"" + String(frequency, 1) + "\",";
    // jsonData += "\"PF\": \"" + String(pf) + "\"";
    // jsonData += "}";

    // axsecure.sendData(jsonData, "receive_data");
}
