#include "Axsecure.h"

Axsecure::Axsecure(const char *server, int port) : _server(server), _port(port) {}

bool Axsecure::sendData(String data, const char *subAddress) {
    // String url = _server+String(":")+String(_port);
    String serverStr = String(_server);  // Convert const char* to String
    String url;

    // Check if the server URL starts with "http://"
    if (serverStr.startsWith("http://")) {
        // Concatenate server, port, and subAddress
        url = serverStr + ":" + String(_port) + "/"+ String(subAddress);
    } else {
        // Directly use the server and subAddress without the port
        url = serverStr + "/"+String(subAddress);
    }

    // if (subAddress) {
    //     url += "/";
    //     url += subAddress;
    // }
    Serial.println(url);
    _httpClient.begin(url);
    _httpClient.addHeader("Content-Type", "application/json"); // Set content type as application/json
   
    int httpCode = _httpClient.POST(data);
    _httpClient.end();
    return httpCode == HTTP_CODE_OK;
}

String Axsecure::receiveData(const char *subAddress) {
    String url = _server;
    if (subAddress) {
        url += "/";
        url += subAddress;
    }
    _httpClient.begin(url);
    int httpCode = _httpClient.GET();
    String response = _httpClient.getString();
    _httpClient.end();
    if (httpCode == HTTP_CODE_OK) {
        return response;
    } else {
        return "";
    }
}
