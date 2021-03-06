#include <Arduino.h>
#include <stdint.h>
#include <SD.h>
#include <Ethernet.h>

#include <RequestHandler.h>
#include "Debug/Info.h"
#include "settings.h"

uint8_t mac[] = { MAC_ADDRESS };
IPAddress ip(IP_ADDRESS);

EthernetServer server(80);
RequestHandler requestHandler;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9200);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB port only

    infoLnF("Starting...");

    if (!SD.begin(SD_CS)) {
        infoLnF("SD card initialization failed!");
        while (true) delay(10); // stop (no sd card to load data from)
    }

    Ethernet.begin(mac, ip);

    // check if failed because of no hardware
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        infoLnF("Ethernet shield was not found...");
        while (true) delay(10); // stop (no ethernet shield to use)
    }

    // check if failed because it isn't connected
    if (Ethernet.linkStatus() == LinkOFF) {
        infoLnF("Ethernet cable is not connected...");
        while (true) delay(10); // stop (not connected to anything)
    }

    infoLnF("Beginning server...");
    server.begin();

    infoF("Ip address: ");
    infoLn(Ethernet.localIP());
}

void loop() {
    // listen for an incoming client
    EthernetClient client = server.available();

    // if there is no client, do nothing
    if (!client) return;

    // prepare for new request
    requestHandler.resetValues();

    // enable builtin led while handling request
    digitalWrite(LED_BUILTIN, HIGH);

    requestHandler.handleRequest(&client);

    // request is finished, so turn off the led
    digitalWrite(LED_BUILTIN, LOW);
}