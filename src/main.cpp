#include <Arduino.h>
#include <Ethernet.h>

#include <stdint.h>
#include "settings.h"

uint8_t mac[] = { MAC_ADDRESS };
IPAddress ip(IP_ADDRESS);

EthernetServer server(80);

void setup() {
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

    Serial.println(F("There is a client!"));

    Serial.print(F("with ip: "));
    Serial.println(client.remoteIP());
}