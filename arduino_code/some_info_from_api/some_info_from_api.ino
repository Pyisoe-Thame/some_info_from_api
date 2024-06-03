#include "config.h"  // path added
#include "myJsonUtil.h"
#include "serialPrinter.h" 
#include "some_info_from_api.h"

#include <WiFi.h>

void setup() 
{
  Serial.begin(115200);
  delay(100);  // add a small delay for serial to fully init
  Serial.print("Connecting to Wi-Fi");
  WiFi.mode(WIFI_STA);  // wifi mode persist across power on/off cycles
  connectToWiFi();
  // deep sleep, longer response can be tolerted bcz no immediate reponse is needed 
  WiFi.setSleep(WIFI_PS_MIN_MODEM);  // minimum periodic sleep
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP: ");  // IPAddress is complex data type (not a string)
  Serial.println(WiFi.localIP());  // so leave it like this
  Serial.print('\n');  // putchar('\n'); on serial monitor for next set of info

  JsonDocument * geo_doc = goFetchJson( ipinfoUrl, ipinfoToken);
  if(geo_doc)  // or if( geo_doc != nullptr)
  {
    geo_serial(geo_doc);  // show geolocation on serial monitor
    const char* city = (*geo_doc)["city"];  // city name, e.g. "Mandalay" for Mandalay
    const char* country = (*geo_doc)["country"];  // country code, e.g. "mm" for Myanmar
    //  with info obtained from geolocation api
    weatherApiUrlBase = weatherApiUrlBase + city + "," + country + "&appid=" + apiKey + "&units=metric";
    const char* timezone = (*geo_doc)["timezone"];
    dateTimeApiUrlBase += timezone;
    delete(geo_doc);
  }
  else  // without geolocation data, no timezone and weather cannot be determined
  {
    Serial.println("Error: Geolocation data could not be fetched!");
    Serial.println("Halting further execution...");
    while(true)  // run an inifinte loop doing nothing
      delay(2000);  // you may also blink the LED to indicate ERROR
  }  
  
  // fetch time, set time
  JsonDocument * dt_doc = goFetchJson(dateTimeApiUrlBase.c_str());  // just simply taken the c_str content (not the whole object with additional attributes)
  if(dt_doc)  // or if( dt_doc != nullptr)
  { 
    tmElementsPtr_t tm = getDateTimefromDoc(dt_doc);
    if(tm)  // or if( tm != nullptr)  
    {
      time_t this_time = makeTime(*tm);  // tm eletments to unix timestamp
      setTime(this_time);  // set time with unix timestamp
      // dt_serial(tm);  // show according time on serial monitor
      dt_serial();  // show system's time on serial monitor
      delete tm;  // release the dyna alloc mem
    }
  }
  delete dt_doc;  // can delete even nullptr
}

void loop() 
{
  if( WiFi.status() != WL_CONNECTED)  // check again if the device is connected to Wi-Fi
    connectToWiFi();
  JsonDocument * wth_doc = goFetchJson(weatherApiUrlBase.c_str());  // just simply taken the c_str content (not the whole object with additional attributes)
  if(wth_doc)  // or if( wth_doc != nullptr)
    wth_serial(wth_doc);  // show weather guess on serial monitor
  delete wth_doc;  // no worries, even nullptr is safe to delete
  WiFi.disconnect(true);  // true to turn WiFi-radio hardware off after disconnect
  WiFi.mode(WIFI_OFF);  // set the WIFI_OFF mode as precaution to ensure WiFi-radio off
  delay(6000);  // delay 10 min before the next request
  WiFi.mode(WIFI_STA);  // change back to WiFi station mode
}

void connectToWiFi()
{
  WiFi.begin( ssid, password);  // Connect to Wi-Fi
  while( WiFi.status() != WL_CONNECTED) 
  {
    delay(500);  // a good rate to print a dot
    Serial.print(".");
  }
  Serial.print("\nSignal Strength: ");  // do prinf really consumes more mem?
  Serial.println(WiFi.RSSI());
}

tmElementsPtr_t getDateTimefromDoc( const JsonDocument * dt_doc)
{
  const char * dateTime = (*dt_doc)["datetime"];
  // Serial.print("Date and Time info: ");  
  // Serial.println(dateTime);  // print ISO 8601 type date-time string (raw data)

  int year, month, day, hour, minute, second;  // declare, divide & deploy
  sscanf( dateTime, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &minute, &second);

  tmElementsPtr_t tm = new tmElements_t;  // create a ptr to dyna alloc obj so it persists
  if(tm)
  {
    tm -> Year = CalendarYrToTm(year);  // same as year - 1970, tmElements_t counts year from 1970
    tm -> Month = month;
    tm -> Day = day;
    tm -> Hour = hour;
    tm -> Minute = minute;
    tm -> Second = second;
    return tm;
  }
  else
    // mem alloc has failed. print some error
    return nullptr;
}

