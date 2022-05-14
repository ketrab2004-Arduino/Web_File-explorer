#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "settings.h"

#if DEBUG
#   define debug(x)      Serial.print(x)
#   define debugF(x)     Serial.print(F(x))
#   define debugLn(x)    Serial.println(x)
#   define debugLnF(x)   Serial.println(F(x))
#else
#   define debug(x)
#   define debugF(x)
#   define debugLn(x)
#   define debugLnF(x)
#endif

#define debugVar(s,v) debugF(s);debugLn(v)

#endif