#include "serialPrinter.h"

void geo_serial( const JsonDocument * geo_doc)
{
  const char* loc = (*geo_doc)["loc"];
  const char* city = (*geo_doc)["city"];
  const char* region = (*geo_doc)["region"];
  const char* country = (*geo_doc)["country"];

  Serial.printf("Location: %s\n", loc);
  Serial.printf("City: %s\n", city);
  Serial.printf("Region: %s\n", region);
  Serial.printf("Country: %s\n", country);  // no reason to put '\n' next line
  Serial.print('\n');  // putchar('\n'); on serial monitor for next set of info
}

void wth_serial( const JsonDocument * wth_doc)
{
  const float temperature = (*wth_doc)["main"]["temp"];
  const int humidity = static_cast<int> ((*wth_doc)["main"]["humidity"]);
  const char* weatherDescription = (*wth_doc)["weather"][0]["description"];

  Serial.printf("Temperature: %.2f\n", temperature);
  Serial.printf("Humidity: %d\n", humidity);
  Serial.printf("Description: %s\n", weatherDescription);
  Serial.print('\n');  // putchar('\n'); on serial monitor for next set of info
}

void dt_serial( const tmElementsPtr_t tm)  // print the dateTime value of tm in a format
{
    // month names array
    const char* months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    Serial.println("Current date and time: ");

/*
    print date & time in my favourite format; Dec/27/2023, 23:47:13
    print system's date & time if no argument is given
*/
    if(!tm)  // or if( tm == nullptr)
    {
        Serial.print( months[month() - 1]);  // -1 to index month array
        Serial.print("/");
        if( day() < 10) Serial.print("0");  // 0 filler for one digit numbers
        Serial.print( day());
        Serial.print("/");
        Serial.print( year());
        Serial.print(", ");
        if( hour() < 10) Serial.print("0");  // 0 filler for one digit numbers
        Serial.print( hour());
        Serial.print(":");
        if( minute() < 10) Serial.print("0");  // 0 filler for one digit numbers
        Serial.print( minute());
        Serial.print(":");
        if( second() < 10) Serial.print("0");  // 0 filler for one digit numbers
        Serial.print( second());
    }
    else  // tm is given
    {
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
    }
    Serial.println('\n');  // double putchar('\n'); on serial monitor for next set of info
}

