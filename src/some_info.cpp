/*
  Author : Pyae Sone Heine
  Modified : May/22/2024
*/

#include <../include/some_info.h>

const char* ssid     = "WiFi Name";
const char* password = "WiFi Password";

// geolocation data site 'ipinfo' namely 
const char* ipinfoUrl = "http://ipinfo.io/json";
const char* ipinfoToken = "ipinfo_token"; // API key for ipinfo ( if available)

// OpenWeatherMap API details
const char* apiKey = "weather_api_key"; // Ensure this is correct
const char* city = "mandalay";
const char* country = "mm"; // e.g., "us" for the United States

// OpenWeatherMap API URL
String weatherApiUrl = String("http://api.openweathermap.org/data/2.5/weather?q=") + city + "," + country + "&appid=" + apiKey + "&units=metric";

void setup() 
{
  Serial.begin(115200);
  
  WiFi.begin( ssid, password);  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  while( WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() 
{
  if( WiFi.status() == WL_CONNECTED)  // Check if the device is connected to Wi-Fi
  { 
    JsonDocument * wth_doc = goFetchJson(weatherApiUrl.c_str());  // just simply taken the c_str content (not the whole object with additional attributes)
    int temperature = (*wth_doc)["main"]["temp"];
    int humidity = (*wth_doc)["main"]["humidity"];
    const char* weatherDescription = (*wth_doc)["weather"][0]["description"];
    delete wth_doc;
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Description: ");
    Serial.println(weatherDescription); 

    JsonDocument * geo_doc = goFetchJson( ipinfoUrl, ipinfoToken);
    const char* loc = (*geo_doc)["loc"];
    String city = (*geo_doc)["city"];
    String region = (*geo_doc)["region"];
    String country = (*geo_doc)["country"];
    String org = (*geo_doc)["org"];
    delete geo_doc;

    Serial.println("Location: " + String(loc));
    Serial.println("City: " + city);
    Serial.println("Region: " + region);
    Serial.println("Country: " + country);
    Serial.println("Organization: " + org);
  } 
  else 
    Serial.println("Wi-Fi not connected");
  
  delay(600000);  // delay 10 min before the next request
}

JsonDocument * goFetchJson( const char * apiUrl, const char * token)  // token is optional
{
  HTTPClient http;
  http.begin(apiUrl); // specify the URL
  if( token != nullptr)
    http.addHeader("Authorization", String("Bearer ") + token);  // add the autho header if token is provided

  int httpCode = http.GET(); // make the request

  if( httpCode > 0)  // check for the returning code
  { 
    String payload = http.getString();
    http.end();  // free the resources as soon as demanded payload is got
    // Serial.println("Fetched Data: ");
    // Serial.println(payload);  // print the response payload from server
      
    DynamicJsonDocument* doc = new DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson( *doc, payload);
    if(error)  // just  deserialization error handling
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      delete doc;
      return nullptr;  // return an empty doc since NULL is not allowed
    }
    return doc;
  } 
  else
  {
    Serial.println("Error on HTTP request");
    http.end();  // free the resources
    return nullptr;
  }
}

