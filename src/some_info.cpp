/*
  Author : Pyae Sone Heine
  Modified : May/24/2024
*/

#include <../include/some_info.h>

static const char* ssid     = "WiFi Name";  // these variables are used only here for WiFi
static const char* password = "WiFi Password";  // please don't come hack my devices through home wifi

// geolocation data site 'ipinfo' namely 
const char* ipinfoUrl = "http://ipinfo.io/json";
const char* ipinfoToken = "ipinfo_token";  // API key for ipinfo ( optional/if_available)

// OpenWeatherMap API details
const char* apiKey = "weather_api_key";  // free but api key needed
// global url ( to be concat with required info later)
String weatherApiUrl = "http://api.openweathermap.org/data/2.5/weather?q=";  

// WorldTimeAPI details ( no api key required)
// global url ( to be concat with required info later)
String dateTimeApiUrl = "http://worldtimeapi.org/api/timezone/";

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
  Serial.println("\nConnected to Wi-Fi\n");

  JsonDocument * geo_doc = goFetchJson( ipinfoUrl, ipinfoToken);
  if( geo_doc != nullptr)
    geo_serial(geo_doc);  // show geolocation on serial monitor
  const char* city = (*geo_doc)["city"];  // city name, e.g. "Mandalay" for Mandalay
  const char* country = (*geo_doc)["country"];  // country code, e.g. "mm" for Myanmar
  //  with info obtained from geolocation api
  weatherApiUrl = weatherApiUrl + city + "," + country + "&appid=" + apiKey + "&units=metric";
  const char* timezone = (*geo_doc)["timezone"];
  dateTimeApiUrl += timezone;
  delete(geo_doc);
}

void loop() 
{
  if( WiFi.status() == WL_CONNECTED)  // Check if the device is connected to Wi-Fi
  { 
    JsonDocument * wth_doc = goFetchJson(weatherApiUrl.c_str());  // just simply taken the c_str content (not the whole object with additional attributes)
    if( wth_doc != nullptr)
      wth_serial(wth_doc);  // show weather guess on serial monitor
    delete wth_doc;

    JsonDocument * dt_doc = goFetchJson(dateTimeApiUrl.c_str());  // just simply taken the c_str content (not the whole object with additional attributes)
    if( dt_doc != nullptr)
    { 
      tmElementsPtr_t tm = getDateTimefromDoc(dt_doc);
      if( tm != nullptr)
      {
        time_t this_time = makeTime(*tm);  // tm eletments to unix timestamp
        setTime(this_time);  // set time with unix timestamp
        // dt_serial(tm);  // show weather guess on serial monitor
        serialPrintDateTime();
        delete tm;  // release the dyna alloc mem
      }
    }
    delete dt_doc;
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

void geo_serial( const JsonDocument * geo_doc)
{
  const char* loc = (*geo_doc)["loc"];
  const char* city = (*geo_doc)["city"];
  const char* region = (*geo_doc)["region"];
  const char* country = (*geo_doc)["country"];

  Serial.print("Location: ");
  Serial.println(loc);  // String can concat in C++ but char* needs C function
  Serial.print("City: ");
  Serial.println(city);
  Serial.print("Region: ");
  Serial.println(region);
  Serial.print("Country: ");
  Serial.println(country);
  Serial.print('\n');  // putchar('\n'); on serial monitor for next set of info
}

void wth_serial( const JsonDocument * wth_doc)
{
  const int temperature = static_cast<int> ((*wth_doc)["main"]["temp"]);  // typecast float to int with more reliability
  const int humidity = static_cast<int> ((*wth_doc)["main"]["humidity"]);  // typecast float to int with more reliability
  const char* weatherDescription = (*wth_doc)["weather"][0]["description"];

  Serial.print("Temperature: ");
  Serial.println(temperature);  // this is arduino's usage
  Serial.print("Humidity: ");
  Serial.println(humidity);  // this is arduino's usage
  Serial.print("Description: ");
  Serial.println(weatherDescription);  // String can concat in C++ but char* needs C function
  Serial.print('\n');  // putchar('\n'); on serial monitor for next set of info
}

tmElementsPtr_t getDateTimefromDoc( const JsonDocument * dt_doc)
{
  const char * dateTime = (*dt_doc)["datetime"];
  // Serial.print("Date and Time info: ");  
  // Serial.println(dateTime);  // print ISO 8601 type date-time string (raw data)

  int year, month, day, hour, minute, second;  // declare, divide & deploy
  sscanf( dateTime, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &minute, &second);

  tmElementsPtr_t tm = new tmElements_t;  // create a ptr to dyna alloc obj so it persists
  if( tm == nullptr)
  {
    // mem alloc failed, Serial.print if Serial avaialable
    return nullptr;
  }
  tm -> Year = CalendarYrToTm(year);  // same as year - 1970, tmElements_t counts year from 1970
  tm -> Month = month;
  tm -> Day = day;
  tm -> Hour = hour;
  tm -> Minute = minute;
  tm -> Second = second;
  return tm;
}

void dt_serial( const tmElementsPtr_t tm)
{
  // month names array
  const char* months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  // print date_time in my favourite format; Dec/27/2023, 23:47:13
  Serial.print( months[(*tm).Month - 1]);  // -1 to index month array
  Serial.print("/");
  if( (*tm).Day < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( (*tm).Day);
  Serial.print("/");
  Serial.print(tmYearToCalendar( (*tm).Year));
  Serial.print(", ");
  if( (*tm).Hour < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( (*tm).Hour);
  Serial.print(":");
  if( (*tm).Minute < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( (*tm).Minute);
  Serial.print(":");
  if( (*tm).Second < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( (*tm).Second);
  Serial.println('\n');  // double putchar('\n'); on serial monitor for next set of info
}

void serialPrintDateTime()
{
  // month names array
  const char* months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  Serial.print("Current date and time: ");
  // print date_time in my favourite format; Dec/27/2023, 23:47:13
  Serial.print( months[month() - 1]);  // -1 to index month array
  Serial.print("/");
  if( day() < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( day());
  Serial.print("/");
  Serial.print(year());
  Serial.print(", ");
  if( hour() < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( hour());
  Serial.print(":");
  if( minute() < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( minute());
  Serial.print(":");
  if( second() < 10) Serial.print("0");  // 0 filler for one digit numbers
  Serial.print( second());
  Serial.println('\n');  // double putchar('\n'); on serial monitor for next set of info
}

