#ifndef SOME_INFO_H
#define SOME_INFO_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

JsonDocument * goFetchJson( const char * apiUrl, const char * token = nullptr);
void geo_serial( const JsonDocument * geo_doc);
void wth_serial( const JsonDocument * wth_doc);
void dt_serial( const JsonDocument * dt_doc);

#endif