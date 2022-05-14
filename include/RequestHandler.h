#ifndef __REQUESTHANDLER_H__
#define __REQUESTHANDLER_H__

#include <Arduino.h>
#include <stdint.h>
#include <SD.h>
#include <Ethernet.h>
#include <StringCache.h>

#include "settings.h"
#include "Debug/Info.h"
#include "Debug/Debug.h"

enum RequestType: uint8_t {
    UNSET,
    UNKNOWN,

    GET,
    POST,
    PUT,
    HEAD,
    DELETE,
    PATCH,
    OPTIONS,
    CONNECT,
    TRACE
};

enum RouteType: uint8_t {
    UNKNOWN_ROUTE,
    OTHER,

    API,
    API_FILE,
    API_FOLDER
};

class RequestHandler
{
protected:
    EthernetClient *client;

    StringCache store;
    bool storeHasUrl = false;

    bool lastCharIsNewline = true;
    RequestType requestType = UNSET;
    RouteType routeType = UNKNOWN_ROUTE;

public:
    void resetValues();

    RequestType getRequestType();
    static RequestType getRequestType(StringCache &store);

    void handleRequest(EthernetClient *client);

protected:
    void handleRequestCharacter(char c, unsigned int &textIndex);
    void handleReply();

    void handleAPIFileReply();
    void handleAPIFolderReply();

    void sendDefaultHeaders();

};

#endif