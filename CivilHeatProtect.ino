//#include <Wire.h>
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

#define WIFI_MAX_WAITING_TIME 15000 // How much time we wait for WiFi connection 

/* Serial Monitor messages definition */

#define SEPARATOR "=============================="
#define DOT "."
#define IN_AND_OUT_SENSORS "IN-house and Outdoor Humidity & temperature Sensors"
#define ACCESSING_WIFI "Accessing WiFi"
#define WIFI_CONNECTED "WiFi connection successful!"
#define WIFI_NOT_CONNECTED "WiFi connection failed! Please check settings."
#define INITIALIZING_SD "Initializing SD card..."
#define SD_CARD_INITIALIZED "SD card initialization successful!"
#define SD_CARD_NOT_INITIALIZED "SD card failed! Please check connection."
#define IN_CONDITIONS "IN-Side Conditions  => "
#define OUT_CONDITIONS "OUT-Side Conditions  => "

/* LCD messages definition */

#define LCD_ACCESSING_WIFI "Accessing WiFi"
#define LCD_WIFI_CONNECTED "WiFi successful"
#define LCD_WIFI_NOT_CONNECTED "WiFi failed"
#define LCD_INITIALIZING_SD "Initializing SD"
#define LCD_SD_CARD_INITIALIZED "SD initialized"
#define LCD_SD_CARD_NOT_INITIALIZED "SD failed"
#define LCD_EMPTY ""

/* Macro Function for printing to user (Serial & LCD) */

#define PRINT_TO_USER(serial_message, lcd_message) print_to_user(serial_message, lcd_message)

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht1(DHTPIN_IN, DHTTYPE_IN);
DHT dht2(DHTPIN_OUT, DHTTYPE_OUT);

const int Ld1_bluePin = D0;
const int Ld1_greenPin = D2;
const int Ld1_redPin=D3;
const int Ld2_redPin= D4;
const int delay_time = 500;
const long timeframe = 6000;
const int readspermin = 10;

String ssid = "Dimitris";
String password = "2jJKBCYdT!";
WiFiClient client; 
// ThingSpeak information
unsigned long channelID = 598750;
char* writeAPIKey = "3WAVULEKX06N4LNM";
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
float temp,temp2; //Stores temperature value
String settings_file = "settings.txt";
File myFile;


void setup() {
  // put your setup code here, to run once:
  pinMode(Ld1_redPin, OUTPUT);
  pinMode(Ld1_greenPin, OUTPUT);
  pinMode(Ld1_bluePin, OUTPUT);
  pinMode(Ld2_redPin, OUTPUT);
  pinMode(DHTPIN_IN, INPUT);
  pinMode(DHTPIN_OUT, INPUT);     

  Serial.begin(115200);

  if (readSD()){
    if (wificonnect())
      flag = 0;  
    else
      flag = 1; 
  }
  else{
    if (wificonnect())
      flag = 2;  
    else
      flag = 3;
  }
  
  dht1.begin();
  dht2.begin();
  
  switchoff();   
  PRINT_TO_USER(IN_AND_OUT_SENSORS, LCD_EMPTY);
  delay(7000);//Delay to let system boot Wait before accessing Sensor  

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
 //   write2TSData( channelID , df1, avg1C , df2, avg1H , df3 ,DI ); 
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
int write2TSData( long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, float field2Data, unsigned int TSField3, long field3Data ){

  ThingSpeak.setField( TSField1, field1Data );
  ThingSpeak.setField( TSField2, field2Data );
  ThingSpeak.setField( TSField3, field3Data );
   
  int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
  if ( writeSuccess ){ 
    Serial.println( "Tmp: " + String(field1Data) + "C Hum: " + String(field2Data) + "% DI: " + String(field3Data) + " written to Thingspeak." );  
  }
  return writeSuccess;
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


bool readSD(){
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
  PRINT_TO_USER(INITIALIZING_SD, LCD_INITIALIZING_SD);
  if (!SD.begin(4)) {
    PRINT_TO_USER(SD_CARD_NOT_INITIALIZED, LCD_SD_CARD_NOT_INITIALIZED);
    return false;
   // while (1);
  }
  PRINT_TO_USER(SD_CARD_INITIALIZED, LCD_SD_CARD_INITIALIZED);

  myFile = SD.open(settings_file);
  if (myFile) {
    Serial.println("Read contents of " + settings_file);

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " +  settings_file);
    return false;
  }
  PRINT_TO_USER(SEPARATOR, LCD_EMPTY); 
  return true;
}


bool   wificonnect(){

  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println();
  PRINT_TO_USER(ACCESSING_WIFI, LCD_ACCESSING_WIFI);
  
  uint32_t tStart = millis(); // WiFi starts the attempt to connect
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis()-tStart) >= WIFI_MAX_WAITING_TIME)
      return false;
      
    delay(500);
    PRINT_TO_USER(DOT, LCD_EMPTY);
  }
  PRINT_TO_USER(WIFI_CONNECTED, LCD_WIFI_CONNECTED);
//  ThingSpeak.begin( client );
  return true;
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
  if (serial_message == IN_CONDITIONS || serial_message == OUT_CONDITIONS || serial_message == DOT)
    Serial.print(serial_message);
  else
    Serial.println(serial_message);
    
//  lcd.clear();
//  lcd.print(lcd_message);
}
