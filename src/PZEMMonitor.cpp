#include "PZEMMonitor.h"
#include "Axsecure.h" // Include the Axsecure header file

// Define the constructor for PZEMMonitor
PZEMMonitor::PZEMMonitor(int rxPin, int txPin, HardwareSerial& serial) : pzem(serial, rxPin, txPin) {}

// Define the function to read and print values
void PZEMMonitor::readAndPrintValues() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  if(isnan(voltage) || isnan(current) || isnan(power) || isnan(energy) || isnan(frequency) || isnan(pf)) {
    Serial.println("Error reading PZEM values");
  } 
  else {
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);
  }
  Serial.println();

  // Create an instance of the Axsecure class
  Axsecure axsecure("your_server_ip_or_domain", 80); // Replace "your_server_ip_or_domain" with your server's IP address or domain name and port

  // Example of sending data to the server
  String data = "Voltage: " + String(voltage) + "V\n" +
                "Current: " + String(current) + "A\n" +
                "Power: " + String(power) + "W\n" +
                "Energy: " + String(energy, 3) + "kWh\n" +
                "Frequency: " + String(frequency, 1) + "Hz\n" +
                "PF: " + String(pf);
  
  axsecure.sendData(data);
}
