#include "RequestHandler.h"

void RequestHandler::resetValues()
{
    client = nullptr;

    // fill store with spaces
    store.fill();
    storeHasUrl = false;

    lastCharIsNewline = true;
    requestType = UNSET;
    routeType = UNKNOWN_ROUTE;
}


RequestType RequestHandler::getRequestType()
{
    return getRequestType(store);
}
/* static */RequestType RequestHandler::getRequestType(StringCache &s)
{
    // can't do switch case with char *, char[] nor String

    if (s == "GET")
        return GET;

    else if (s == "POST")
        return POST;

    else if (s == "PUT")
        return PUT;

    else if (s == "HEAD")
        return HEAD;

    else if (s == "DELETE")
        return DELETE;

    else if (s == "PATCH")
        return PATCH;

    else if (s == "OPTIONS")
        return OPTIONS;

    else if (s == "CONNECT")
        return CONNECT;

    else if (s == "TRACE")
        return TRACE;

    /* else */return RequestType::UNKNOWN;
}


void RequestHandler::handleRequest(EthernetClient *client)
{
    this->client = client;

    infoLnF("There is a client!");
    infoVar("with ip: ", client->remoteIP());

    unsigned int textIndex = 0;

    while (client->connected()) {
        if (!client->available()) continue; // skip this iteration

        char c = client->read(); // read the next character in the request
        debugWrite(c); // write to Serial if DEBUG is enabled

        handleRequestCharacter(c, textIndex);

        // if we've gotten to the end of the line (received a newline character)
        // and the line is blank, the http request has ended (so we can send a reply)
        if (c == '\n' && lastCharIsNewline) {
            handleReply();
            break;
        }

        switch (c)
        {
        // if we've gotten to the end of the line, mark it as blank
        case '\n':
            lastCharIsNewline = true;

        // ignore carriage returns
        case '\r':
            break;

        // if we've gotten to a character, mark the line as not blank
        default:
            lastCharIsNewline = false;
            break;
        }
    }

    // give the web browser time to receive the data
    delay(1);

    // close the connection
    client->stop();
    infoLnF("Client disconnected");
}

void RequestHandler::handleRequestCharacter(char c, unsigned int &textIndex)
{
    if (requestType == UNSET) {
        if (c == ' ') { // if end of request type (space)
            store.temporarySnip(textIndex);
            requestType = getRequestType(); // convert string request type to enum RequestType

            store.fill(); // reset store (also resets temporary snip)
            textIndex = 0; // reset index in store

        } else {
            store.addAt(textIndex, c);
            textIndex++;
        }
    } else {
        if (storeHasUrl) return;

        if (routeType == UNKNOWN_ROUTE) {
            if (c == '/' && textIndex > 1) { // if char is /, but ignore first /
                if (store.startsWith("/api")) {
                    routeType = API;

                    textIndex = 1; // current char is now /
                    store.fill();
                    store.addAt(0, '/'); // set first char to /
                } else {
                    routeType = OTHER;
                }
            } else {
                store.addAt(textIndex, c);
                textIndex++;
            }

        } else if (routeType == API) {
            if (c == '/' || c == ' ') { // if end of route (space or /)
                if (store.startsWith("/file")) {
                    routeType = API_FILE;

                    textIndex = 1; // first char is /
                    store.fill();
                    store.addAt(0, '/'); // set first char to /

                } else if (store.startsWith("/folder")) {
                    routeType = API_FOLDER;

                    textIndex = 1; // first char is /
                    store.fill();
                    store.addAt(0, '/'); // set first char to /

                } else {
                    routeType = UNKNOWN_ROUTE;
                }

            } else {
                store.addAt(textIndex, c);
                textIndex++;
            }
        } else if (routeType == API_FILE || routeType == API_FOLDER) {
            if (c == ' ') { // if end of the rest of the url (space)
                storeHasUrl = true;
                store.temporarySnip(textIndex + 1);

            } else {
                store.addAt(textIndex, c);
                textIndex++;
            }
        }
    }
}

void RequestHandler::handleReply()
{
    if (requestType == UNKNOWN) {
        client->println(F("HTTP/1.1 400 Bad Request"));
        sendDefaultHeaders();
        return;
    } else if (requestType != GET) {
        client->println(F("HTTP/1.1 405 Method Not Allowed"));
        sendDefaultHeaders();
        return;
    }

    switch (routeType)
    {
    case UNKNOWN:
    case API: // API means /api with nothing after it, so no chosen api "method"
        client->println(F("HTTP/1.1 400 Bad Request"));
        sendDefaultHeaders();
        break;

    case API_FILE:
        handleAPIFileReply();
        break;
    case API_FOLDER:
        handleAPIFolderReply();
        break;

    // case OTHER: // compiler complains for "duplicate case value"
    default:
        client->println(F("HTTP/1.1 200 OK"));
        sendDefaultHeaders();

        client->println();

        client->println(F("<!DOCTYPE HTML>"));
        client->println(F("<html>"));
        client->println(F("<head>"));
        client->println(F("<title>Arduino Ethernet Shield</title>"));
        client->println(F("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"/api/file" WEB_FOLDER "/FAVICON.ICO\">"));
        client->println(F("</head>"));
        client->println(F("<body>"));
        client->println(F("<h1>Hello World!</h1>"));
        client->println(F("</body>"));
        client->println(F("</html>"));
        break;
    }
}

void RequestHandler::handleAPIFileReply()
{
    File f = SD.open(store.c_str(), FILE_READ);

    if (!f) {
        client->println(F("HTTP/1.1 404 Not Found"));
        sendDefaultHeaders();
        return;
    }

    if (f.isDirectory()) {
        client->println(F("HTTP/1.1 409 Conflict"));
        client->println(F("X-Error: Folder is not a file"));
        sendDefaultHeaders();

        f.close();
        return;
    }

    client->println(F("HTTP/1.1 200 OK"));
    // client->println(F("Content-Type: application/json")); // idk lol
    sendDefaultHeaders();

    client->println();

    for (uint32_t i = 0; i < f.size(); i++) { // go through file
        client->write(f.read()); // write file to client
    }
}
void RequestHandler::handleAPIFolderReply()
{
    File f = SD.open(store.c_str(), FILE_READ);

    if (!f) {
        client->println(F("HTTP/1.1 404 Not Found"));
        client->print(F("debug: "));
        client->println(store.c_str());
        sendDefaultHeaders();
        return;
    }

    if (!f.isDirectory()) {
        client->println(F("HTTP/1.1 409 Conflict"));
        client->println(F("X-Error: File is not a folder"));
        sendDefaultHeaders();

        f.close();
        return;
    }

    client->println(F("HTTP/1.1 200 OK"));
    client->println(F("Content-Type: application/json"));
    sendDefaultHeaders();

    client->println();
    client->println(F("["));

    for (uint16_t fileCount = 0; true; fileCount++) {
        File entry = f.openNextFile();

        if (!entry) break;

        client->println(fileCount > 0 ? F(",{") : F("{")); // add , before { if not first file

        #define var(s,v) print(F("\"" s "\": \"")); client->print(v); client->println(F("\","))

        client->var("name", entry.name());
        client->var("size", entry.size());
        client->var("isDirectory", entry.isDirectory() ? F("true") : F("false"));
        client->var("position", entry.position());

        client->print(F("\"index\": \""));
        client->print(fileCount);
        client->println(F("\"")); // no comma on last property

        #undef var // undefine var macro

        client->println(F("}"));

        entry.close();
    }

    client->println(F("]"));
}


void RequestHandler::sendDefaultHeaders()
{
    client->println(F("Server: Arduino Ethernet Shield"));

    client->print(F("Server-uptime: "));
    client->print(millis());
    client->println(F(" ms"));

    client->println(F("X-Powered-By: cpp"));

    client->println(F("Connection: close")); // close the connection after completing the request
}
