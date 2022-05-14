#ifndef __DEBUG_INFO_H__
#define __DEBUG_INFO_H__

#include "settings.h"

#if INFO
#   define info(v)      Serial.print(v)
#   define infoF(s)     Serial.print(F(s))
#   define infoLn(v)    Serial.println(v)
#   define infoLnF(s)   Serial.println(F(s))
#else
#   define info(v)
#   define infoF(s)
#   define infoLn(v)
#   define infoLnF(s)
#endif

#define infoVar(s,v) infoF(s);infoLn(v)

#endif