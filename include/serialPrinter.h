#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include <ArduinoJson.h>
#include <TimeLib.h>

void geo_serial( const JsonDocument * geo_doc);
void wth_serial( const JsonDocument * wth_doc);
void dt_serial( const tmElementsPtr_t tm = nullptr);

#endif

