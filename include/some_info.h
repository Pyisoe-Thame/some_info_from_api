#ifndef SOME_INFO_H
#define SOME_INFO_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

JsonDocument * goFetchJson( const char * apiUrl, const char * token = nullptr);

#endif