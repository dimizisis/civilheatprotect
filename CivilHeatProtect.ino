//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <SPI.h>  // Library for SD Card
#include <SD.h>   // Library for SD Card
#include <DHT.h>  // Library for DHT sensors
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <IniFile.h>
#include <ArduinoJson.h>

//Constants ARDUINO UNO
/*
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
*/
//Constants D1 MINI
/*
#define D3 0
#define D4 2
#define RX 3
#define D2 4
#define D1 5
#define D6 12
#define D7 13
#define D8 14
#define D9 15
#define D0 16*/

//Constants D1

#define D0 3    // RGB LED - pin 1
#define D1 1    // Read serial monitor // Don't connect anything
#define D2 16   // RGB LED - pin 2
#define D3 5    // RGB LED - pin 3
#define D4 4    // Red LED
#define D5 14   // Inside Sensor
#define D6 12   // Don't connect anything
#define D7 13   // Outside Sensor
#define D8 0    // Wifi Transmit
#define D9 2    // Wifi Receive
#define D10 15  // SD ChipSelect
#define D11 13  // SD MOSI
#define D12 12  // SD MISO
#define D13 14  // SD SCK

#define DHT11PIN D5     // what pin we're connected to
#define DHT22PIN D7     // what pin we're connected to
#define DHTTYPE DHT22
#define CONFIGURATION_FILENAME "config.ini"
#define BUFFER_SIZE 80

/* Configuration file sections */

#define SYSTEM_SETTINGS "SSETTINGS"
#define USER_SETTINGS "USETTINGS"

#define DATA_FILENAME "data.csv"

#define WIFI_MAX_WAITING_TIME 20000 // How much time we wait for WiFi connection
#define SYSTEM_BOOT_DELAY 7000
#define STANDARD_DELAY_TIME 3000
#define SERVER_MINIMUM_DELAY_TIME 15000

/* Sensor Limits */

#define DHT22_MAX_TEMP 80.00
#define DHT22_MIN_TEMP -40.00

#define DHT22_MAX_HUM 100.00
#define DHT22_MIN_HUM 0.00

#define DHT11_MAX_TEMP 50.00
#define DHT11_MIN_TEMP 0.00

#define DHT11_MAX_HUM 90.00
#define DHT11_MIN_HUM 20.00

/* Serial Monitor messages definition */

#define SEPARATOR "=============================="
#define DOT "."
#define ACCESSING_WIFI "Accessing WiFi"
#define WIFI_CONNECTED "WiFi connection successful!"
#define WIFI_NOT_CONNECTED "WiFi connection failed! Please check settings."
#define INITIALIZING_SD "Initializing SD card..."
#define SD_CARD_INITIALIZED "SD card initialization successful!"
#define SD_CARD_NOT_INITIALIZED "SD card failed! Please check connection."
#define IN_CONDITIONS "IN-Side Conditions  => "
#define OUT_CONDITIONS "OUT-Side Conditions  => "
#define IN_SENSORS "IN-house Humidity & temperature Sensors"
#define OUT_SENSORS "Outdoor Humidity & temperature Sensors"
#define IN_AND_OUT_SENSORS "IN-house and Outdoor Humidity & temperature Sensors"
#define CONFIG_FILE_NOT_EXIST "Configuration file doesn't seem to exist."
#define DEFAULT_SETTINGS "Using Default Settings..."
#define UNAVAILABLE_DHT11 "Sensor DHT11 unavailable..."
#define UNAVAILABLE_DHT22 "Sensor DHT22 unavailable..."
#define SENSORS_UNAVAILABLE "All sensors unavailable..."
#define RESET "Please reset..."
#define OUT_OF_BOUND_ERROR "Temperature or Humidity out of bounds"
#define USING_DHT11 "Using DHT11 sensor..."
#define USING_DHT22 "Using DHT22 sensor..."
#define TEMPERATURE "Temperature (Celsius): "
#define HUMIDITY "Humidity: "
#define API_CONNECTION_FAILED "Connection with API has failed"
#define PARSE_OBJECT_FAILED "parseObject() failed"
#define GETTING_DATA "Getting data from API..."

/* LCD messages definition */

#define LCD_ACCESSING_WIFI "Accessing WiFi"
#define LCD_WIFI_CONNECTED "WiFi successful"
#define LCD_WIFI_NOT_CONNECTED "WiFi failed"
#define LCD_INITIALIZING_SD "Initializing SD"
#define LCD_SD_CARD_INITIALIZED "SD initialized"
#define LCD_SD_CARD_NOT_INITIALIZED "SD failed"
#define LCD_CONFIG_FILE_NOT_EXIST "config.ini Fail"
#define LCD_DEFAULT_SETTINGS "Default Settings"
#define LCD_UNAVAILABLE_DHT11 "DHT11 error..."
#define LCD_UNAVAILABLE_DHT22 "DHT22 error..."
#define LCD_SENSORS_UNAVAILABLE "Sensors error..."
#define LCD_RESET "Reseting..."
#define LCD_OUT_OF_BOUND_ERROR "Temp/Hum error"
#define LCD_USING_DHT11 "Using DHT11..."
#define LCD_USING_DHT22 "Using DHT22..."
#define LCD_TEMPERATURE "TEMP (C): "
#define LCD_HUMIDITY "HUM (%): "
#define LCD_EMPTY ""
#define LCD_API_CONNECTION_FAILED "API conn. failed"
#define LCD_PARSE_OBJECT_FAILED "Parsing failed"
#define LCD_GETTING_DATA "API Fetching"

/* Macro Function for printing to user (Serial & LCD) */

#define PRINT_TO_USER(serial_message, lcd_message, flag) print_to_user(serial_message, lcd_message, flag)

void print_config_file_error(uint8_t e, bool eol = true); /* Prints specific error in case of .ini file failure. */

// Initialize DHT sensor for normal 16mhz Arduino
int main_sensor=22; // 22 for DHT22, 11 for DHT11. By default we use sensor DHT22
DHT dht1(DHT22PIN, DHTTYPE);
//DHT dht2(DHT11PIN, DHTTYPE);

const int Ld1_bluePin = D0;
const int Ld1_greenPin = D2;
const int Ld1_redPin=D3;
const int Ld2_redPin= D4;
int delay_time;
long timeframe;
int readsperframe;

String wifi_ssid;
String wifi_pass;
WiFiClient client;
// IOT server information
long channelID;
String iot_server;
char* writeAPIKey;
// Weather API information
char weather_server[BUFFER_SIZE];
String weather_api_key;
String city_id;
unsigned int df1 = 1;  // Data Field to write temperature data
unsigned int df2 = 2;  // Data Field to write Humidity data
unsigned int df3 = 3;  // Data Field to write distress index (in) data
unsigned int df4 = 4;  // Data Field to write distress index (out) data

//Variables
int flag; /* if SD card & WiFi OK, flag = 0
             if SD card OK, WiFi failed to connect, flag = 1
             if SD card failed to initialize & WiFi failed to connect, flag = 2 */
int chk;
float hum,hum2;  //Stores humidity value
float temp,temp2; //Stores temperature value;
float humOutside,tempOutside; //Stores humidity and temperature from OpenWeather-API
File myFile;
char buffer[BUFFER_SIZE];
int read_sd;

void setup() {
  // put your setup code here, to run once:

  pinMode(Ld1_redPin, OUTPUT);
  pinMode(Ld1_greenPin, OUTPUT);
  pinMode(Ld1_bluePin, OUTPUT);
  pinMode(Ld2_redPin, OUTPUT);
  pinMode(DHT22PIN, INPUT);
  //pinMode(DHT11PIN, INPUT);

  Serial.begin(115200);

  if ((read_sd = readSD()) == 0){

    if (wificonnect()){
      flag = 0;
      PRINT_TO_USER(IN_AND_OUT_SENSORS, LCD_EMPTY, true);
    }
    else{
      flag = 1;
      PRINT_TO_USER(IN_SENSORS, LCD_EMPTY, true);
    }
  }
  else if (read_sd == 1) {
    get_default_settings();
    PRINT_TO_USER(DEFAULT_SETTINGS, LCD_DEFAULT_SETTINGS, true);
    delay(STANDARD_DELAY_TIME);
    flag = 3;
  }
  else{
    get_default_settings();
    PRINT_TO_USER(DEFAULT_SETTINGS, LCD_DEFAULT_SETTINGS, true);
    delay(STANDARD_DELAY_TIME);
      flag = 1; // We have SD card available, but not configuration file and WiFi (SD Card OK & WiFi failure)
      PRINT_TO_USER(IN_SENSORS, LCD_EMPTY, true);
  }

  dht1.begin();
//  dht2.begin();

  if (main_sensor == 22)
    PRINT_TO_USER(USING_DHT22, LCD_USING_DHT22, true);
  else if (main_sensor == 11)
    PRINT_TO_USER(USING_DHT11, LCD_USING_DHT11, true);
  delay(STANDARD_DELAY_TIME);

  switchoff();
  delay(SYSTEM_BOOT_DELAY); // Delay to let system boot Wait before accessing Sensor
}

void loop() {

    float avg1C=0,avg1H=0,avg2C=0,avg2H=0, DI_temp_in, DI_temp_out;
    bool sensor_success=false,GET_success=false;
    int DI_in, DI_out;

    sensor_success = get_data_from_sensors(flag, &avg1C, &avg2C, &avg1H, &avg2H);
    if (!sensor_success)
      ESP.restart();

    (main_sensor == 22) ? DI_temp_in = calc_DI_temp(avg1C, avg1H) : DI_temp_in = calc_DI_temp(avg2C, avg2H);
      
    DI_in = calc_DI(DI_temp_in);
    led_status(DI_in);

    delay(STANDARD_DELAY_TIME);

    if (flag==0){
      (main_sensor == 22) ? DI_temp_out = calc_DI_temp(avg1C, avg1H) : DI_temp_out = calc_DI_temp(avg2C, avg2H);
      DI_out = calc_DI(DI_temp_out);
      GET_success = get_from_OpenWeatherMapAPI();
      delay(STANDARD_DELAY_TIME*2);
      write_to_server( iot_server, channelID , df1, avg1C , df2 , avg1H , df3 , DI_in , df4 , DI_out );
      delay(SERVER_MINIMUM_DELAY_TIME);

    }

}
