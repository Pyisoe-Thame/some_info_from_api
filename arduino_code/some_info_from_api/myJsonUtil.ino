#include "myJsonUtil.h"
#include <HTTPClient.h>

JsonDocument * goFetchJson( const char * apiUrl, const char * token)  // token is optional
{
  HTTPClient http;
  http.begin(apiUrl); // specify the URL
  if( token)  // if( token != nullptr)
    http.addHeader("Authorization", String("Bearer ") + token);  // add the autho header if token is provided

  int httpCode = http.GET(); // make the request

  if( httpCode > 0)  // check for the returning code
  { 
    String payload = http.getString();
    http.end();  // free the resources as soon as demanded payload is got
    // if serial monitor is on, can print ugly json data with the following two lines
    // Serial.println("Fetched Data: ");
    // Serial.println(payload);  // print the response payload from server
      
    DynamicJsonDocument* doc = new DynamicJsonDocument(1024);
    if( doc == nullptr) 
    {
      Serial.println("Failed to allocate memory for JsonDocument");
      return nullptr;
    }
    DeserializationError error = deserializeJson( *doc, payload);
    if(error)  // just deserialization error handling
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

