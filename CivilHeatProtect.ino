#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>  // Library for SD Card
#include <DHT.h>  // Library for DHT sensors
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <IniFile.h>
#include <ArduinoJson.h>

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

//#define DHT22PIN D7     // what pin we're connected to
#define DHTPIN D9     // what pin we're connected to
#define DHTTYPE DHT11
#define CONFIGURATION_FILENAME "config.ini"
#define BUFFER_SIZE 80

/* Configuration file sections */

#define SYSTEM_SETTINGS "SSETTINGS"
#define USER_SETTINGS "USETTINGS"

#define THINGSPEAK_SERVER "api.thingspeak.com"

#define WIFI_MAX_WAITING_TIME 20000 // How much time we wait for WiFi connection
#define SYSTEM_BOOT_DELAY 7000
#define STANDARD_DELAY_TIME 3000
#define SERVER_MINIMUM_DELAY_TIME 15000

/* Sensor Limits */

#define DHT_MAX_TEMP 60.00
#define DHT_MIN_TEMP -20.00

#define DHT_MAX_HUM 95.00
#define DHT_MIN_HUM 5.00

/* Serial Monitor messages definition */

#define EMPTY ""
#define SEPARATOR "================"
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
#define UNAVAILABLE_dht1 "Sensor dht1 unavailable..."
#define UNAVAILABLE_DHT22 "Sensor DHT22 unavailable..."
#define SENSORS_UNAVAILABLE "All sensors unavailable..."
#define RESET "Please reset..."
#define OUT_OF_BOUND_ERROR "Temperature or Humidity out of bounds"
#define USING_dht1 "Using dht1 sensor..."
#define USING_DHT22 "Using DHT22 sensor..."
#define TEMPERATURE "Temperature (Celsius): "
#define HUMIDITY "Humidity: "
#define API_CONNECTION_FAILED "Connection with API has failed"
#define PARSE_OBJECT_FAILED "parseObject() failed"
#define GETTING_DATA "Getting data from API..."
#define DATA_SEND_FAIL "Data send failed"

/* LCD messages definition */

#define LCD_SAMPLING "Sampling..."
#define LCD_PROJECT_NAME "CivilHeatProtect"
#define LCD_ACCESSING_WIFI "Accessing WiFi"
#define LCD_WIFI_CONNECTED "WiFi successful"
#define LCD_WIFI_NOT_CONNECTED "WiFi failed"
#define LCD_INITIALIZING_SD "Initializing SD"
#define LCD_SD_CARD_INITIALIZED "SD initialized"
#define LCD_SD_CARD_NOT_INITIALIZED "SD failed"
#define LCD_CONFIG_FILE_NOT_EXIST "config.ini Fail"
#define LCD_DEFAULT_SETTINGS "Default Settings"
#define LCD_UNAVAILABLE_dht1 "dht1 error..."
#define LCD_UNAVAILABLE_DHT22 "DHT22 error..."
#define LCD_SENSORS_UNAVAILABLE "Sensors error..."
#define LCD_RESET "Reseting..."
#define LCD_OUT_OF_BOUND_ERROR "Temp/Hum error"
#define LCD_USING_dht1 "Using dht1..."
#define LCD_USING_DHT22 "Using DHT22..."
#define LCD_TEMPERATURE "TEMP (C): "
#define LCD_HUMIDITY "HUM (%): "
#define LCD_EMPTY ""
#define LCD_API_CONNECTION_FAILED "API conn. failed"
#define LCD_PARSE_OBJECT_FAILED "Parsing failed"
#define LCD_GETTING_DATA "API Fetching"
#define LCD_DATA_SEND_FAIL "Data send failed"

/* Macro Function for printing to user (Serial & LCD) */
#define PRINT_TO_USER(serial_message, lcd_message, cursor1, cursor2, write_to_lcd, clear_lcd) print_to_user(serial_message, lcd_message, cursor1, cursor2, write_to_lcd, clear_lcd)

/* Set the LCD address to 0x27 for a 16 chars and 2 line display */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* Prints specific error in case of .ini file failure. */
void print_config_file_error(uint8_t e, bool eol = true); 

/* Initializing DHT sensor */
DHT dht(DHTPIN, DHTTYPE);

/* Initializing WiFi Client that will be used to send data to IOT Server */
WiFiClient client;

/* Defining type, to keep all info about sensor (limits, type & etc), that will be needed in loop */

typedef struct{
  int type = DHTTYPE;
  int pin = DHTPIN;
  float max_temp = DHT_MAX_TEMP;
  float min_temp = DHT_MIN_TEMP;
  float max_hum = DHT_MAX_HUM;
  float min_hum = DHT_MIN_HUM;
} Sensor;

/* Defining pins */

const int Ld1_bluePin = D0;
const int Ld1_greenPin = D2;
const int Ld1_redPin = D3;
const int Ld2_redPin = D5;

/* Configuration variables (System Settings) */

String iot_server;
int delay_time;
long time_frame;
int reads_per_frame;
String weather_api_key;
char weather_server[BUFFER_SIZE];

/* Configuration variables (User Settings) */

String wifi_ssid;
String wifi_pass;
long channel_id;
char* write_api_key;
String city_id;
String zip_code;

/* Data field numbers (used in IOT Server writing) */

unsigned int field1 = 1;  // Data Field to write temperature data
unsigned int field2 = 2;  // Data Field to write Humidity data
unsigned int field3 = 3;  // Data Field to write distress index (in) data
unsigned int field4 = 4;  // Data Field to write distress index (out) data

/* Extra Variables */

int flag; /* if SD card & WiFi OK, flag = 0
             if SD card OK or failed to initialize & WiFi failed to connect, flag = 1 */
             
char buffer[BUFFER_SIZE]; // We use buffer to transfer data from config file

int read_sd; /* if everything OK, read_sd = 0
                if SD card failed to initialize, read_sd = 1
                if config file not exist, read_sd = 2
                if config file exists but not valid, read_sd = 3 */

Sensor sensor; // All variables inside sensor are pre-initialized

void setup() {

  set_pins();

  /* Initializing and turn on backlight */
  initialize_lcd();

  /* Title of project at the very beggining */
  PRINT_TO_USER(EMPTY, LCD_PROJECT_NAME, 0, 0, true, true); 
  PRINT_TO_USER(EMPTY, SEPARATOR, 0, 1, true, false); 

  delay(STANDARD_DELAY_TIME);

  Serial.begin(115200);

  /* DHT sensor begins */
  dht.begin();

  /* Checking SD & WiFi condition and sets flag */
  get_flag();

  /* Switching off LEDs */
  switchoff();

  /* Delay that will let system to boot properly */
  delay(SYSTEM_BOOT_DELAY); // Delay to let system boot Wait before accessing Sensor
}

void loop() {

    float avgC=0,avgH=0, min_temp, max_temp, min_hum, max_hum, DI_temp_in, DI_temp_out;
    float temp_outside, hum_outside; // Stores humidity and temperature from OpenWeather-API
    bool sensor_success=false,GET_success=false;
    int DI_in, DI_out;
    int obs=0; // Observations

    PRINT_TO_USER(EMPTY, LCD_SAMPLING, 0, 0, true, true);

    /* Get samples from sensor */
    sensor_success = get_data_from_sensor(&avgC, &avgH, &obs, &min_temp, &max_temp, &min_hum, &max_hum);

    /* If sensors did not worked as expected, reset device */
    if (!sensor_success)
      ESP.restart();

    /* Get the average temperature & humidity, in order to calculate DI (in) */
    get_averages(&avgC, &avgH, min_temp, max_temp, min_hum, max_hum, &obs);

    /* Get DI (in) using the formula */
    DI_temp_in = calc_DI_temp(avgC, avgH);

    /* Get DI (in) as integer (1-6) */
    DI_in = calc_DI(DI_temp_in);

    /* Set LED color */
    led_status(DI_in);

    delay(STANDARD_DELAY_TIME);

    if (flag == 0){

      /* Get data (temperature & humidity) from OpenWeatherMap API (to calculate DI (out)) */
      GET_success = get_from_OpenWeatherMapAPI(&temp_outside, &hum_outside);
      
      delay(STANDARD_DELAY_TIME);
      
      /* Get DI (out) using the formula */
      DI_temp_out = calc_DI_temp(temp_outside, hum_outside);

      /* Get DI (out) as integer (1-6) */
      DI_out = calc_DI(DI_temp_out);
      
      (void) write_to_server(iot_server, avgC, avgH, DI_in, DI_out);
      print_to_user("T:"+String(avgC,1)+"  H:"+String(avgH,1)+"%", "T:"+String(avgC,1)+"  H:"+String(avgH,1)+"%", 0, 0, true, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user("DI-IN:"+String(DI_in)+" DI-OUT:"+String(DI_out), "DI-IN:"+String(DI_in)+" DI-OUT:"+String(DI_out), 0, 1, true, false);
      delay(SERVER_MINIMUM_DELAY_TIME);
    }
    else{
      
      /* Just printing inside conditions & DI */
      
      print_to_user("T:"+String(avgC,1)+"  H:"+String(avgH,1)+"%", "T:"+String(avgC,1)+"  H:"+String(avgH,1)+"%", 0, 0, true, true);
      delay(STANDARD_DELAY_TIME);
      
      print_to_user("DI-IN:"+String(DI_in), "DI-IN:"+String(DI_in), 0, 1, true, false);
      delay(STANDARD_DELAY_TIME);
    }

}

/*
 * Function:  get_flag 
 * --------------------
 * sets flag to 0 if SD successfully initializes & WiFi connects or sets flag to 1 if something goes wrong with SD or WiFi
 *
 *  returns: nothing, void func
 * 
 */

void get_flag(){
  
  if ((read_sd = readSD()) == 0)
    ((wifi_connect())) ? flag = 0 : flag = 1;
  else{
    get_default_settings();
    PRINT_TO_USER(DEFAULT_SETTINGS, LCD_DEFAULT_SETTINGS, 0, 0, true, true);
    delay(STANDARD_DELAY_TIME);
      flag = 1;
  }
  
}

/*
 * Function:  get_averages 
 * --------------------
 * calculates average temperature & humidity values and sets them to avgC & avgH. Removes maximum & minimum temperature & humidity values, and subtracts observations with 2
 * 
 * avgC: pointer to temperature variable, the one that will be used to calculate the average temp
 * avgH: pointer to humidity variable, the one that will be used to calculate the average hum
 * min_temp: pointer to minimum temperature variable (from all observations)
 * max_temp: pointer to maximum temperature variable (from all observations)
 * min_hum: pointer to minimum humidity variable (from all observations)
 * max_hum: pointer to maximum humidity variable (from all observations)
 * obs: pointer to number of observations variable, the one that will be used to divide avgC, avgH with
 * 
 * returns: nothing, void func
 * 
 */

void get_averages(float * avgC, float * avgH, float min_temp, float max_temp, float min_hum, float max_hum, int * obs){

  /* Remove min & max temperature & humidity values from averages (for better sampling) */
  (*avgC) -= max_temp;
  (*avgC) -= min_temp;
  (*avgH) -= max_hum;
  (*avgH) -= min_hum;
  (*obs) -= 2;
  (*avgC) /= (*obs);
  (*avgH) /= (*obs);
  
}
