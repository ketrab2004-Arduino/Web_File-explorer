#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "settings.h"

#if DEBUG
#   define debug(v)      Serial.print(v)
#   define debugF(s)     Serial.print(F(s))
#   define debugLn(v)    Serial.println(v)
#   define debugLnF(s)   Serial.println(F(s))
#   define debugWrite(v) Serial.write(v)
#else
#   define debug(v)
#   define debugF(s)
#   define debugLn(v)
#   define debugLnF(s)
#   define debugWrite(v)
#endif

#define debugVar(s,v) debugF(s);debugLn(v)

#endif