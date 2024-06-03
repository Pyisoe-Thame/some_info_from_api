#include "myJsonUtil.h"
#include <HTTPClient.h>

JsonDocument * goFetchJson( const char * apiUrl, const char * token)  // token is optional
{
  HTTPClient http;
  http.begin(apiUrl); // specify the URL
  if( token)  // or if( token != nullptr)
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
    if(doc)  // or if( doc != nullptr)
    {
      DeserializationError error = deserializeJson( *doc, payload);
      if(!error)  // just deserialization error handling
        return doc;
      else  // error happened
      {
        Serial.printf("Deserialization failed: %s\n", error.c_str());
        delete doc;
        http.end();
        return nullptr;  // return an empty doc since NULL is not allowed
      }
    }
    else  // new doc allocation failed
    {
      Serial.println("Failed to allocate memory for JsonDocument");
      return nullptr;
    }
  } 
  else
  {
    Serial.printf("Error in fetching data: %s\n", http.errorToString(httpCode).c_str());
    http.end();  // free the resources
    return nullptr;
  }
}

