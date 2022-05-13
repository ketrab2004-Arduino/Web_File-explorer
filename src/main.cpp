#include <Arduino.h>
#include <Ethernet.h>

#include <stdint.h>
#include "settings.h"

uint8_t mac[] = { MAC_ADDRESS };
IPAddress ip(IP_ADDRESS);

EthernetServer server(80);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // sketch can hang with an SD card, solution:
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    Serial.begin(9200);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB port only

    Serial.println(F("Starting..."));

    // attempt to connect to the internet (with DHCP for an IP address)
    Ethernet.begin(mac, ip);

    // check if failed because of no hardware
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println(F("Ethernet shield was not found..."));
        while (true) delay(10); // stop (no ethernet shield to use)
    }

    // check if failed because it isn't connected
    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println(F("Ethernet cable is not connected..."));
        while (true) delay(10); // stop (not connected to anything)
    }

    Serial.println(F("Beginning server..."));
    server.begin();

    Serial.print(F("Ip address: "));
    Serial.println(Ethernet.localIP());
}

void loop() {
    // listen for an incoming client
    EthernetClient client = server.available();

    // if there is no client, do nothing
    if (!client) return;

    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(F("There is a client!"));

    Serial.print(F("with ip: "));
    Serial.println(client.remoteIP());

    bool currentLineIsBlank = true;
    while (client.connected()) {
        if (!client.available()) continue; // skip this iteration

        char c = client.read(); // read the current character
        Serial.write(c);

        // if we've gotten to the end of the line (received a newline character)
        // and the line is blank, the http request has ended (so we can send a reply)
        if (c == '\n' && currentLineIsBlank) {
            // respond to the client
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Server: Arduino Ethernet Shield"));
            client.println(F("X-Powered-By: cpp"));
            client.println(F("Content-Type: text/html"));
            client.println(F("Connection: close")); // close the connection after completing the request

            client.println();

            client.println(F("<!DOCTYPE HTML>"));
            client.println(F("<html>"));
            client.println(F("<body>"));
            client.println(F("<h1>Hello World!</h1>"));
            client.println(F("</body>"));
            client.println(F("</html>"));
            break;
        }


        // if we've gotten to the end of the line, mark it as blank
        if (c == '\n') currentLineIsBlank = true;

        // if we've gotten to a character, mark the line as no longer blank (\r is not not blank)
        else if (c != '\r') currentLineIsBlank = false;
    }

    // give the web browser time to receive the data
    delay(1);

    // close the connection
    client.stop();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(F("Client disconnected :("));
}