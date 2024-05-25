
# some_info_from_api

NodeMCU-32S is used in this project. The purpose of doing this project is to learn how to get some useful information from http api. In this project, geolocation info, weather info and dateTime info are obtained and shown on serial monitor. 

- I got this idea at the time when I can not afford the overall cost of GPS sensor, temperature sensor, RTC, etc. 

-  I hope this would become a helping hand to students' IoT projects.


## Deployment

For Arduino IDE users, some libraries like ArduinoJson.h and TimeLib.h would need to be installed with library manager.

PlatformIO users can directly upload and monitor onto NodeMCU-32S since the library installations are initialized at the lib_deps section of platformio.ini config file. They can change the upload_port, upload_speed, monitor_port, and monitor_speed according to their likings. 

Older devices tend to work with 9600 baud rate. As you can see, the baud rate here is set to 115200 so do not forget to see only the 115200 monitor. Otherwise, you won't see the output correctly.



## API Reference

#### Get geolocaion info from IPinfo

```http
  GET http://ipinfo.io/json
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `api_key` | `string` | **Optional**. Your API key |

#### Get weather info from OpenWeatherMap

```http
  GET http://api.openweathermap.org/data/2.5/weather?q=${city},${country}&appid=${apiKey}&units=${unit}
```

| Parameter | Type     | Description                            |
| :-------- | :------- | :--------------------------------------|
| `city`    | `string` | **Required**. e.g. Mandalay            |
| `country` | `string` | **Required**. e.g. jp for Japan        |
| `api_key` | `string` | **Required**. Your API key             |
| `unit`    | `string` | **Optional**. metric or imperial etc.  |

#### Get date and time info from WorldTimeAPI
```http
  curl http://worldtimeapi.org/api/timezone/${timezone_string}
```

| Parameter        | Type     | Description                     |
| :--------------- | :--------| :-------------------------------|
| `timezone_string`| `string` | **Required**. e.g. "Asia/Yangon"|

## License

[MIT](https://choosealicense.com/licenses/mit/)


## Screenshots

![Serial Output](https://github.com/Pyisoe-Thame/some_info_from_api/blob/main/images/serial_output.PNG)
*Figure 1: Serial output of the application.*

