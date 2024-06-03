#ifndef MAIN_H
#define MAIN_H

#include <TimeLib.h>
#include <ArduinoJson.h>

void connectToWiFi();
tmElementsPtr_t getDateTimefromDoc( const JsonDocument * dt_doc);

#endif