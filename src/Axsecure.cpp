#include "Axsecure.h"

Axsecure::Axsecure(const char* server, int port) : _server(server), _port(port) {}

bool Axsecure::sendData(String data) {
    if (!_client.connect(_server, _port)) {
        Serial.println("Connection to server failed");
        return false;
    }

    _client.print("POST / HTTP/1.1\r\n");
    _client.print("Host: ");
    _client.print(_server);
    _client.print("\r\n");
    _client.print("Content-Type: application/json\r\n");
    _client.print("Content-Length: ");
    _client.print(data.length());
    _client.print("\r\n\r\n");
    _client.print(data);

    delay(100); // Give the server time to respond

    _client.stop();
    return true;
}

String Axsecure::receiveData() {
    if (!_client.connect(_server, _port)) {
        Serial.println("Connection to server failed");
        return "";
    }

    _client.print("GET / HTTP/1.1\r\n");
    _client.print("Host: ");
    _client.print(_server);
    _client.print("\r\n\r\n");

    delay(100); // Give the server time to respond

    String response = "";
    while (_client.available()) {
        char c = _client.read();
        response += c;
    }

    _client.stop();
    return response;
}
