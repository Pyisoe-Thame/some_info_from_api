#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

extern const char* ssid; 
extern const char* password;  

// geolocation data site 'ipinfo' namely 
extern const char* ipinfoUrl;
extern const char* ipinfoToken;  // API key for ipinfo ( optional/if_available)

// OpenWeatherMap API details
extern const char* apiKey;  // free but api key needed
extern String weatherApiUrlBase;  // String to concat with necessary info later

// WorldTimeAPI details ( no api key required)
extern String dateTimeApiUrlBase;  // String to concat with necessary info later

#endif