#include "StringCache.h"

void StringCache::addAt(uint16_t i, char c)
{
    if (i == length())
        operator+=(c); // add character to end of string using +=

    else if(i >= length()) {
        // add spaces until the string is long enough for setting the character at the correct position
        for (uint16_t j = 0; j < i - length() -1; j++) {
            operator+=(' '); // add a space to the end of the string
        }

        operator+=(c); // add the given character
    }

    else
        setCharAt(i, c); // set the character at the given position
}

// set every character in the string to the given character
void StringCache::fill(char c = ' ')
{
    for (uint16_t i = 0; i < length(); i++) {
        setCharAt(i, c);
    }
}
