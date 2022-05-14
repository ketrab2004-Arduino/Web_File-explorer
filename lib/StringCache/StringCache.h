#ifndef __STRING_CACHE_H__
#define __STRING_CACHE_H__

#include <Arduino.h>
#include <stdint.h>


class StringCache: public String
{
protected:
    char snippedChar = '\0';
    char *snipEnd = nullptr;

public:
    using String::String; // use String's constructors

    void addAt(uint16_t index, char character);

    void fill(char character = ' ');

    void temporarySnip(uint16_t length);
    void undoTemporarySnip();
    bool isSnipped() { return snipEnd != nullptr; }

};


#endif