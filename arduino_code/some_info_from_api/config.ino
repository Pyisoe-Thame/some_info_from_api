#include "config.h"

const char* ssid     = "WiFi Name";  // these variables are used for WiFi 
const char* password = "WiFi Password";  // please don't come hack my devices through home wifi

// geolocation data site 'ipinfo' namely 
const char* ipinfoUrl = "http://ipinfo.io/json";
const char* ipinfoToken = "ipinfo_token";  // API key for ipinfo ( optional/if_available)

// OpenWeatherMap API details
const char* apiKey = "weather_api_key";  // free but api key needed
// to be concat with required info later
String weatherApiUrlBase = "http://api.openweathermap.org/data/2.5/weather?q=";  

// WorldTimeAPI details ( no api key required)
// to be concat with required info later
String dateTimeApiUrlBase = "http://worldtimeapi.org/api/timezone/";

