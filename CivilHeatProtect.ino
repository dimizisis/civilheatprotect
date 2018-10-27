#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <SPI.h>  // Library for SD Card
#include <SD.h>   // Library for SD Card
#include <DHT.h>  // Library for DHT sensors
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <IniFile.h>

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

#define DHTPIN_OUT D5     // what pin we're connected to
#define DHTTYPE_OUT DHT11   // DHT 11 
#define DHTPIN_IN D7     // what pin we're connected to
#define DHTTYPE_IN DHT22   // DHT 22 

#define CONFIGURATION_FILENAME "config.ini"
#define BUFFER_SIZE 80

/* Configuration file sections */

#define SYSTEM_SETTINGS "SSETTINGS"
#define USER_SETTINGS "USETTINGS"

#define WIFI_MAX_WAITING_TIME 20000 // How much time we wait for WiFi connection
#define SYSTEM_BOOT_DELAY 7000

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

/* LCD messages definition */

#define LCD_ACCESSING_WIFI "Accessing WiFi"
#define LCD_WIFI_CONNECTED "WiFi successful"
#define LCD_WIFI_NOT_CONNECTED "WiFi failed"
#define LCD_INITIALIZING_SD "Initializing SD"
#define LCD_SD_CARD_INITIALIZED "SD initialized"
#define LCD_SD_CARD_NOT_INITIALIZED "SD failed"
#define LCD_CONFIG_FILE_NOT_EXIST "config.ini Fail"
#define LCD_DEFAULT_SETTINGS "Default Settings"
#define LCD_EMPTY ""

/* Macro Function for printing to user (Serial & LCD) */

#define PRINT_TO_USER(serial_message, lcd_message) print_to_user(serial_message, lcd_message)

void print_config_file_error(uint8_t e, bool eol = true); /* Prints specific error in case of .ini file failure. */

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht1(DHTPIN_IN, DHTTYPE_IN);
DHT dht2(DHTPIN_OUT, DHTTYPE_OUT);

const int Ld1_bluePin = D0;
const int Ld1_greenPin = D2;
const int Ld1_redPin=D3;
const int Ld2_redPin= D4;
int delay_time;
long timeframe;
int readspermin;

String wifi_ssid;
String wifi_pass;
WiFiClient client; 
// IOT server information
long channelID;
String iot_server;
char* writeAPIKey;
unsigned int df1 = 1;  // Data Field to write temperature data
unsigned int df2 = 2;  // Data Field to write Humidity data
unsigned int df3 = 3;  // Data Field to write distress index data

//Variables
int flag; /* if SD card & WiFi OK, flag = 0
             if SD card OK, WiFi failed to connect, flag = 1
             if SD card failed to initialize, WiFi OK, flag = 2
             if SD card failed to initialize & WiFi failed to connect, flag = 3 */           
int chk;
float hum,hum2;  //Stores humidity value
float temp,temp2; //Stores temperature value;
File myFile;
char buffer[BUFFER_SIZE];
int read_sd;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(Ld1_redPin, OUTPUT);
  pinMode(Ld1_greenPin, OUTPUT);
  pinMode(Ld1_bluePin, OUTPUT);
  pinMode(Ld2_redPin, OUTPUT);
  pinMode(DHTPIN_IN, INPUT);
  pinMode(DHTPIN_OUT, INPUT);     

  Serial.begin(115200);

  if ((read_sd = readSD()) == 0){
      
    if (wificonnect()){
      flag = 0;
      PRINT_TO_USER(IN_AND_OUT_SENSORS, LCD_EMPTY);  
    }
    else{
      flag = 1;
      PRINT_TO_USER(IN_SENSORS, LCD_EMPTY);
    }
  } 
  else if (read_sd == 1) {
    get_default_settings();
    PRINT_TO_USER(DEFAULT_SETTINGS, LCD_DEFAULT_SETTINGS);
    delay(3000);
    if (wificonnect())
      flag = 2;  
    else
      flag = 3;
  }
  else{
    get_default_settings();
    PRINT_TO_USER(DEFAULT_SETTINGS, LCD_DEFAULT_SETTINGS);
    delay(3000);
    if (wificonnect()){
      flag = 0;  // We have SD card available, but not configuration file. (SD Card & WiFi OK)
      PRINT_TO_USER(IN_AND_OUT_SENSORS, LCD_EMPTY);
    }
    else{
      flag = 1; // We have SD card available, but not configuration file and WiFi (SD Card OK & WiFi failure)
      PRINT_TO_USER(IN_SENSORS, LCD_EMPTY);
    }
  }
  
  dht1.begin();
  dht2.begin();
  
  switchoff();   
  delay(SYSTEM_BOOT_DELAY); // Delay to let system boot Wait before accessing Sensor  

}

void loop() {

  // If SD card & WiFi OK
  if (flag == 0){
    
    int s1,s2;
    float avg1C,avg1H,avg2C,avg2H,DI_temp;

    s1=0; s2=0; 
    avg1C=0; avg1H=0; avg2C=0; avg2H=0; DI_temp=0;
    for(int i=0;i<readspermin; i++){
        temp= dht1.readTemperature();
        hum = dht1.readHumidity();  
        temp2= dht2.readTemperature();          
        hum2 = dht2.readHumidity();
        if (isnan(temp)||isnan(hum)) {
           return;
        }else{
          avg1C = avg1C + temp;
          avg1H = avg1H + hum;
          s1=s1+1;
        } 
       
        if (isnan(temp2)||isnan(hum2)) {
          return;
        }else{
          avg2C = avg2C + temp2;
          avg2H = avg2H + hum2;
          s2=s2+1;
        } 
        delay(timeframe/readspermin);//Wait n seconds before accessing sensor again.           
    }

    avg1C = avg1C/s1;
    avg1H = avg1H/s1;
    avg2C = avg2C/s2;
    avg2H = avg2H/s2;
    DI_temp= round(avg1C - 0.55 * (1 - 0.01 * avg1H) * (avg1C - 14.5));
        
   // Read data and store it to variables hum and temp 
    PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
    PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY);
    print_temphum(avg1C,avg1H);
    PRINT_TO_USER(OUT_CONDITIONS, LCD_EMPTY);
    print_temphum(avg2C,avg2H); 
    int DI = calc_DI(DI_temp);
 //   write2TSData( iot_server, channelID , df1, avg1C , df2, avg1H , df3 ,DI ); 
    led_status(DI);
  }

  // If SD card OK, WiFi failed to connect
  else if (flag == 1){

    
  }

  // If SD card failed to initialize, WiFi OK
  else if (flag == 2){
    

    
  }
  
 // If SD card failed to initialize & WiFi failed to connect
  else{
    

    
  }
}

//use this function if you want multiple fields simultaneously
int write2TSData( String server, long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, float field2Data, unsigned int TSField3, long field3Data ){

  if ( server == "ThingSpeak" ){
    ThingSpeak.setField( TSField1, field1Data );
    ThingSpeak.setField( TSField2, field2Data );
    ThingSpeak.setField( TSField3, field3Data );
   
    int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
    if ( writeSuccess ){ 
      Serial.println( "Tmp: " + String(field1Data) + "C Hum: " + String(field2Data) + "% DI: " + String(field3Data) + " written to Thingspeak." );  
    }
    return writeSuccess;
  }
  
}

int calc_DI(int temp){
  if (temp<=21)
    return 1;
  else if (temp<=24)
    return 2;
  else if (temp<=27)
    return 3;
  else if (temp<=29)
    return 4;
  else if (temp<=32)
    return 5;
  else if (temp>32)
    return 6;
}


int readSD(){

  /* 
   *  Returns 0 when everything is OK
   *  Returns 1 when SD card failed to initialize
   *  Returns 2 when configuration file does not exist
   *  Returns 3 when configuration file is not valid
   */
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
  PRINT_TO_USER(INITIALIZING_SD, LCD_INITIALIZING_SD);
  if (!SD.begin(4)) {
    PRINT_TO_USER(SD_CARD_NOT_INITIALIZED, LCD_SD_CARD_NOT_INITIALIZED);
    return 1;
  }
  PRINT_TO_USER(SD_CARD_INITIALIZED, LCD_SD_CARD_INITIALIZED);

  IniFile ini(CONFIGURATION_FILENAME);

   if (!ini.open()) {
    Serial.println();
    Serial.print("Ini file ");
    Serial.print(CONFIGURATION_FILENAME);
    Serial.println(" does not exist");
    return 2;
  }
  Serial.println("Ini file exists");

  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(buffer, BUFFER_SIZE)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    print_config_file_error(ini.getError());
    return 3;
  }

  get_setting_from_file(ini, USER_SETTINGS, "WIFI_SSID");

  wifi_ssid = String(buffer);
  
  get_setting_from_file(ini, USER_SETTINGS, "WIFI_PASS");

  wifi_pass = String(buffer);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "IOT_SERVER");

  iot_server = String(buffer);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "CHANNEL_ID");

  sscanf(buffer, "%ld", &channelID);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "WRITE_API_KEY");

  writeAPIKey = buffer;

  get_setting_from_file(ini, SYSTEM_SETTINGS, "DELAY_TIME");

  sscanf(buffer, "%d", &delay_time);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "TIME_FRAME");

  sscanf(buffer, "%ld", &timeframe);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "READS_PER_MINUTE");

  sscanf(buffer, "%d", &readspermin);

  return 0;

//  myFile = SD.open(CONFIGURATION_FILENAME);
//  if (myFile) {
//    Serial.println("Read contents of ");
//
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening ");
//    return false;
//  }
  return true;
}

void get_setting_from_file(IniFile ini, char* section, char* entry){

  if (ini.getValue(section, entry, buffer, BUFFER_SIZE)) {
    Serial.print("section " + String(section) + " has an entry " + String(entry) +" with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read " + String(entry) + " from section " + String(section) +", error was ");
    print_config_file_error(ini.getError());
  }
  
}

bool   wificonnect(){

  bool connected = false;

  WiFi.begin(String(wifi_ssid).c_str(), String(wifi_pass).c_str());
  Serial.println();
  PRINT_TO_USER(ACCESSING_WIFI, LCD_ACCESSING_WIFI);
  
  uint32_t tStart = millis(); // WiFi starts the attempt to connect
  while (1){
    if ((millis()-tStart) >= WIFI_MAX_WAITING_TIME){
      PRINT_TO_USER(WIFI_NOT_CONNECTED, LCD_WIFI_NOT_CONNECTED);
      break;
    }
    else if (WiFi.status() == WL_CONNECTED){
      connected = true;
      PRINT_TO_USER(WIFI_CONNECTED, LCD_WIFI_CONNECTED);
      break;
    } 
    delay(1000);
    PRINT_TO_USER(DOT, LCD_EMPTY);
  }
//  ThingSpeak.begin( client );
  return connected;
}

void print_temphum(float temp, float hum){
    Serial.print("Current humidity = ");
    Serial.print(hum);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(temp); 
    Serial.println("C  ");   
}

void led_status(int DI){

if (DI==1){
  setColor1(0, 255, 0);   // Green Color
  setColor2(LOW);           // No Color    
}else if (DI==2){  
  setColor1(0, 0, 255); // Blue Color
  setColor2(LOW);         // No Color 
}else if (DI==3){ 
  setColor1(255, 0, 0); // Red Color
  setColor2(LOW);         // No Color 
}else if (DI==4){     
  setColor1(0, 255, 0);   // Green Color
  setColor2(HIGH);         // Red Color    
 }else if (DI==5){  
  setColor1(0, 0, 255);   // Blue Color
  setColor2(HIGH);         // Red Color 
}else if (DI==6){ 
  setColor1(255, 0, 0);   // Red Color
  setColor2(HIGH);         // Red Color 
} 
  delay(delay_time); 
  //switchoff();
}

void switchoff(){
  setColor1(0, 0, 0);   // No Color
  setColor2(LOW);       // No Color 
  delay(2000);  
}

void setColor1(int redValue, int greenValue, int blueValue) {
  analogWrite(Ld1_redPin, redValue);
  analogWrite(Ld1_greenPin, greenValue);
  analogWrite(Ld1_bluePin, blueValue);
}

void setColor2(int redValue) {
  digitalWrite(Ld2_redPin, redValue);
}

void print_to_user(String serial_message, String lcd_message){
  if (serial_message == IN_CONDITIONS || serial_message == OUT_CONDITIONS || serial_message == DOT || serial_message == ACCESSING_WIFI)
    Serial.print(serial_message);
  else
    Serial.println(serial_message);
    
//  lcd.clear();
//  lcd.print(lcd_message);
}

void get_default_settings(){
  iot_server = "ThingSpeak";
  channelID = 598750;
  writeAPIKey = "3WAVULEKX06N4LNM";
  delay_time = 500;
  timeframe = 6000;
  readspermin = 10;

  wifi_ssid="Test";
  wifi_pass="Test";
}

void print_config_file_error(uint8_t e, bool eol){
  switch (e) {
  case IniFile::errorNoError:
    Serial.print("no error");
    break;
  case IniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case IniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case IniFile::errorBufferTooSmall:
    Serial.print("buffer too small");
    break;
  case IniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case IniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case IniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case IniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case IniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}
