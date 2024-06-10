#include "PZEMMonitor.h"
#include "Axsecure.h"
#include "SensorData.h"
// #include <SPIFFS.h>

PZEMMonitor::PZEMMonitor(int rxPin, int txPin, HardwareSerial &serial, LCDDisplay &lcdDisplay)
    : lcd(lcdDisplay)
{
    for (int i = 0; i < NUM_PZEMS; i++)
    {
        pzems[i] = PZEM004Tv30(serial, rxPin, txPin, 0x50 + i);
    }
}

SensorData sensorData;

void PZEMMonitor::logValues()
{
    unsigned long currentTime = millis();
    ReadingData readingData;
    readingData.timestamp = currentTime;
    int added = false;

    for (int i = 0; i < NUM_PZEMS; i++)
    {
        float voltage = pzems[i].voltage();
        float current = pzems[i].current();
        float power = pzems[i].power();
        float energy = pzems[i].energy();
        float frequency = pzems[i].frequency();
        float powerFactor = pzems[i].pf();
        Reading reading;
        if (!isnan(voltage) && !isnan(current) && !isnan(power) && !isnan(energy) && !isnan(frequency) && !isnan(powerFactor))
        {
            reading.setValues(i+1, voltage, current, power, energy, frequency, powerFactor);
            readingData.readings.push_back(reading);
            added = true;
        }
    }
    if(added)
    {
        sensorData.data.push_back(readingData);
        added = false;
    }
        
}

void PZEMMonitor::readAndPrintValues()
{
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
    for (int i = 0; i < NUM_PZEMS; i++)
    {
        float voltage = pzems[i].voltage();
        float current = pzems[i].current();
        float power = pzems[i].power();
        float energy = pzems[i].energy();
        float frequency = pzems[i].frequency();
        float powerFactor = pzems[i].pf();

        // If any value is not a number (NaN), set the flag to true
        if (!isnan(voltage) && !isnan(current) && !isnan(power) && !isnan(energy) && !isnan(frequency) && !isnan(powerFactor))
        {
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
    for (int j = 0; j < 6; j++)
    {
        String attributeLine;
        String title;
        for (int i = 0; i < NUM_PZEMS; i++)
        {
            // Select the attribute based on the current iteration
            float attributeValue;
            switch (j)
            {
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
            if (!isnan(attributeValue))
            {
                attributeLine = String(attributeValue) + " " + attributeLine;
            }
            else
            {
                attributeLine = "NaNa " + attributeLine;
            }
        }
        lcd.displayTitle(title);
        // Display the attribute line on the LCD
        lcd.displayReadings(attributeLine);
        delay(1000); // Wait for 2 seconds before toggling to the next attribute
    }
}

void PZEMMonitor::sendDataAndClear()
{
    logValues();
    // Create an instance of the Axsecure class
    // Axsecure axsecure("http://192.168.137.21", 8000);
    Axsecure axsecure("https://forgerrr.com", 8000);
    String sensorDataJson;
    DynamicJsonDocument doc(1024);          // Adjust the size as needed
    JsonObject root = doc.to<JsonObject>(); // Convert the document to a JsonObject
    sensorData.toJson(root);                // Populate the JsonObject with sensorData
    serializeJson(doc, sensorDataJson);

    // Send the data to the server
    Serial.println(sensorDataJson); // Print the JSON payload for debugging
    bool success = axsecure.sendData(sensorDataJson.c_str(), "receive_data");

    if (success)
    {
        sensorData.clearData();
    }
    else
    {
        Serial.println("Failed to send data to the server");
    }
}