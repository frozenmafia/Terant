#ifndef AXSECURE_H
#define AXSECURE_H

#include <Arduino.h>
#include <HTTPClient.h>

class Axsecure {
public:
    Axsecure(const char *server, int port);
    bool sendData(String data, const char *subAddress = "");
    String receiveData(const char *subAddress = "");

private:
    const char *_server;
    int _port;
    HTTPClient _httpClient;
};

#endif
