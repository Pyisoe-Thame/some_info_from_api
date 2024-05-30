#ifndef MYJSONUTIL_H
#define MYJSONUTIL_H

#include <ArduinoJson.h>

JsonDocument * goFetchJson( const char * apiUrl, const char * token = nullptr);

#endif

