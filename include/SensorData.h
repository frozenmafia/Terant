#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <ArduinoJson.h>
#include <vector>

class Reading {
public:
    int module;
    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float powerFactor;

    Reading() : module(0), voltage(0), current(0), power(0), energy(0), frequency(0), powerFactor(0) {}

    void setValues(int mod, float volt, float curr, float pow, float ener, float freq, float pf) {
        module = mod;
        voltage = volt;
        current = curr;
        power = pow;
        energy = ener;
        frequency = freq;
        powerFactor = pf;
    }

    void fromJson(JsonObject json) {
        module = json["module"].as<int>();
        voltage = json["voltage"].as<float>();
        current = json["current"].as<float>();
        power = json["power"].as<float>();
        energy = json["energy"].as<float>();
        frequency = json["frequency"].as<float>();
        powerFactor = json["powerFactor"].as<float>();
    }

    void toJson(JsonObject& json) const {
        json["module"] = module;
        json["voltage"] = voltage;
        json["current"] = current;
        json["power"] = power;
        json["energy"] = energy;
        json["frequency"] = frequency;
        json["powerFactor"] = powerFactor;
    }
};

class ReadingData {
public:
    unsigned long timestamp;
    std::vector<Reading> readings;

    ReadingData() : timestamp(0) {}

    void toJson(JsonObject& json) const {
        json["timestamp"] = timestamp;
        JsonArray readingsArray = json.createNestedArray("readings");
        for (const auto& reading : readings) {
            JsonObject readingObj = readingsArray.createNestedObject();
            reading.toJson(readingObj);
        }
    }

    void fromJson(JsonObject json) {
        timestamp = json["timestamp"].as<int>();
        JsonArray readingsArray = json["readings"].as<JsonArray>();
        for (JsonVariant reading : readingsArray) {
            Reading r;
            r.fromJson(reading.as<JsonObject>());
            readings.push_back(r);
        }
    }
};

class SensorData {
public:
    String deviceId;
    std::vector<ReadingData> data;

    SensorData() : deviceId("1") {}

    void toJson(JsonObject& json) const {
        json["deviceId"] = deviceId;
        JsonArray dataArray = json.createNestedArray("data");
        for (const auto& readingData : data) {
            JsonObject readingDataObj = dataArray.createNestedObject();
            readingData.toJson(readingDataObj);
        }
    }

    void fromJson(JsonObject json) {
        deviceId = json["deviceId"].as<String>();
        JsonArray dataArray = json["data"].as<JsonArray>();
        for (JsonVariant data : dataArray) {
            ReadingData readingData;
            readingData.fromJson(data.as<JsonObject>());
            this->data.push_back(readingData);
        }
    }

    void clearData(){
        data.clear();
    }
};

#endif // SENSORDATA_H
