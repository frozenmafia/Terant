#ifndef AXSECURE_H
#define AXSECURE_H

#include <Arduino.h>
#include <WiFiClient.h>

class Axsecure{
public:
    Axsecure(const char * server, int port);
    bool sendData(String data);
    String receiveData();

private:
    const char* _server;
    int _port;
    WiFiClient _client;
};

#endif
