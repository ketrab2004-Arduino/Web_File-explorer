#ifndef __STRING_CACHE_H__
#define __STRING_CACHE_H__

#include <Arduino.h>
#include <stdint.h>


class StringCache: public String
{
public:

    void addAt(uint16_t index, char character);

    void fill(char character = ' ');

};


#endif