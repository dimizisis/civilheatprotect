# civilheatprotect

University Project on Internet of Things

![alt text](https://i.imgur.com/DJxwNIo.jpg)

## General Description

Using WeMos D1R1 as a warning system for internal extreme heat (inside temperature, compared with outside temperature). 

### Discomfort Index

DI = 0.4 (td + tw) + 15

![alt_text](https://cdn.24.co.za/files/Cms/General/d/3649/a016d89036844b31a5a88b06060f0f44.jpg)

### Connection with OpenWeatherMap Web Service

Depending on the location of the user (City ID), prices are obtained from the OpenWeatherMap server for the current weather conditions.
Of the values we get, we only keep the temperature and humidity (these are needed to calculate DI).
In case there is no connection to the server, we do not calculate DI Out).

### Data from Sensors

Values are taken from the sensor (Temperature in Celsius, Humidity in percentage) and the correctness of the values is checked (whether it is inside or outside the limits). With data collected, we calculate the DI In (Distress Index In).

### Analyzing & Connection with IOT Server (ThingSpeak)

Scale from 1 (ideal conditions) to 6 (immediate health risk)
The average temperature and humidity are calculated based on the values we received from the sensors.
**Note**: The highest and lowest temperature and humidity values are not taken into account (optimized sampling).
Through the corresponding type, DI In, DI Out are calculated.
After the previous calculations are done, the data is sent to the IOT Server (in our case ThingSpeak).

## Usage

### Hardware Connections

|        | **D1**        |  |                                                             |         **Connection**               |                 |
| ------ | ------------- | --------------- | ----------------------------------------------------------- | --------------------- | --------------- |
| **ΑΑ** | **PIN ID**    | **Description** |                                                             | **Peripheral**        | **PIN ID**      |
| 1      | D0 – GPIO 3   | RX- Receive     |                                                             | **RGB LED**           | Pin 1 – Blue    |
| 2      | D1 – GPIO 1   | TX – Transmit   | Unconnected (In order to get messages to Serial Monitor-PC) |                       |                 |
| 3      | D2 – GPIO 16  |                 |                                                             | **RGB LED**           | Pin 2 – Green   |
| 4      | D3 – GPIO 5   |                 |                                                             | **RGB LED**           | Pin 3 – Red     |
| 5      | D4 – GPIO 4   |                 | Unconnected (Short circuit with D15-SDA)                    |                       |                 |
| 6      | D5 – GPIO 14  |                 |                                                             | **RED LED**           |                 |
| 7      | D6 – GPIO 12  |                 | Unconnected (Short circuit with D12- MISO)                  |                       |                 |
| 8      | D7 – GPIO 13  |                 | Unconnected (Short circuit with D11- MOSI)                  |                       |                 |
| 9      | D8 – GPIO 0   | WiFi – RX       |                                                             |                       |                 |
| 10     | D9 – GPIO 2   | WiFi – TX       |                                                             | DHT11                 | Data            |
| 11     | D10 – GPIO 15 | SS              |                                                             | **SD Reader**         | SC - Chipselect |
| 12     | D11 – GPIO 13 | MOSI            |                                                             | **SD Reader**         | MOSI            |
| 13     | D12 – GPIO 12 | MISO            |                                                             | **SD Reader**         | MISO            |
| 14     | D13 - GPIO 14 | SCK             |                                                             | **SD Reader**         | SCK             |
| 15     | D14 - GPIO 4  | I2C BUS – SCL   |                                                             | **I2C – LCD MONITOR** | SCL             |
| 16     | D15 - GPIO 5  | I2C BUS – SDA   |                                                             | **I2C – LCD MONITOR** | SDA             |

### LED Indications According to Discomfort Index Value

| **DISCOMFORT INDEX** | **Red LED** | **RGB LED** |
| --- | --- | --- |
| 1 | Off | **GREEN** |
| 2 | Off | **BLUE** |
| 3 | Off | **RED** |
| 4 | **RED** | **GREEN** |
| 5 | **RED** | **BLUE** |
| 6 | **RED** | **RED** |

### SD Card

Saved on SD card (.ini file):

1. WiFi connection data (SSID, Password)
2. IOT Server connection data (Server, Channel ID, API Key)
3. OpenWeatherMap API connection data (API Key, Server)
4. Data for the user's location (City ID, Zip Code)
5. Data on the frequency of sampling (Delay Time, Reads per Frame, Time Frame)

If no SD card is detected, default settings are used.

### External Libraries to include

#### IniFile (Parsing & Reading .ini files)
**https://github.com/stevemarple/IniFile**

## Instructions

(i) Open CivilHeatProtect.ino

(ii) CTRL + S for saving the project to its folder & close Arduino IDE

(iii) Put all .ino files to created folder (project's folder)

(iv) Open one of .ino files & you should be able to see all tabs

[Presentation Video](https://drive.google.com/open?id=1v12Vj4M_c8-2nk8seGw2z2mn5uwCNyRZ)
