/*
Firmware LoRa com Rasp Pico W e módulo E220
14/11/2022
Receptor
M0 - 0
M1 -1
*/
#include "Arduino.h"
#include "LoRa_E220.h"
#include <SPI.h>
#include <RP2040_SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS.h>

#define ss Serial1
String Rssi, latitude, longitude;
TinyGPS gps;

#define PIN_SD_MOSI       19
  #define PIN_SD_MISO       16
  #define PIN_SD_SCK        18
  #define PIN_SD_SS         17
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  7 // sda
#define OLED_CLK    6 // sck display 
#define OLED_DC    3 // dc
#define OLED_CS    4 // cs display
#define OLED_RESET 15 // res
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// ---------- Raspberry PI Pico pins --------------
 LoRa_E220 e220ttl(&Serial2, 2, 10, 11); //  RX AUX M0 M1
// -------------------------------------

float flat, flon,data,hora;
int year;
byte month, day, hour, minute, second, hundredths;

void setup() {
  Serial.begin(9600);
 ss.begin(9600);

  // Startup all pins and UART
  e220ttl.begin();

  Serial.println("Start receiving!");
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(10,0);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
   display.clearDisplay();
   Serial.println(TinyGPS::library_version());
   
   
   Serial.print("Initializing SD card with SS = ");  Serial.println(PIN_SD_SS);
  Serial.print("SCK = ");   Serial.println(PIN_SD_SCK);
  Serial.print("MOSI = ");  Serial.println(PIN_SD_MOSI);
  Serial.print("MISO = ");  Serial.println(PIN_SD_MISO);

  if (!SD.begin(PIN_SD_SS)) 
  {
    Serial.println("Initialization failed!");
    return;
  }
  
  Serial.println("Initialization done.");
}



void loop() {
  //Serial.println("Teste");
  #define fileName  "datalog.txt"
   String dataString = "";
   File dataFile = SD.open("datalog.txt", FILE_WRITE);
   bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
   digitalWrite(11,1);
	// If something available
  	ResponseContainer rc = e220ttl.receiveMessageRSSI();
   Serial.println("Receptor LoRa");
   Serial.print("RSSI: "); Serial.println(-(256-rc.rssi), DEC); 
   Rssi = -(256 -rc.rssi); 
   Serial.println(rc.status.getResponseDescription());
		Serial.println(rc.data);
     if (dataFile) {
   dataFile.println("Data:" + String (day) + ","+ String(month) +String(year) + "," + String
     (hour -3) +"," + String (minute) + "," + String (second) + "Rssi:" + String(Rssi));
     
     /*dataFile.println("Rssi:" + String(Rssi));
     dataFile.println( String(5)  + ",");
     */
    dataFile.close();
    // print to the serial port too:
     Serial.println("Data:" + String (day) + ","+ String(month) +String(year) + "," + String
     (hour -3) +"," + String (minute) + "," + String (second) + "Rssi:" + String(Rssi));
  }
display.clearDisplay();
  
   display.drawRoundRect(0,0, 118, 55, 0, WHITE);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.setTextSize(2);
  display.print("FEEC");
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawRoundRect(0,0, 118, 55, 0, WHITE);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(5,25);
  display.print("RSSI:" + (String) Rssi);
  display.display();  
   // if the file is available, write to it:
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    latitude =  String(flat,5);
    Serial.print(latitude);
    Serial.print(" LON=");
    longitude =  String(flon,5);
    Serial.print(longitude);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        day, month , year, (hour -3), minute, second);
    Serial.print(sz);
     display.clearDisplay();
  display.drawRoundRect(0, 0, 122, 60, 0, WHITE);
  display.setTextColor(WHITE);

    display.setCursor(5,10);
  display.setTextSize(1);
  display.print(String(day) + "-" + (String) month +  "-" +(String) year + " " +(String)(hour -3) +":" +(String) minute +":" +(String) second);
   display.setCursor(5,35);
  display.setTextSize(1);
  display.print("Lat:" +(String)latitude );
   display.setCursor(5,45);
  display.setTextSize(1);
  display.print("Long:" +(String) longitude );
  display.setCursor(26,25);
  display.setTextSize(2);
  display.print("-");
  display.setCursor(30,25);
  display.setTextSize(2);
  display.print(month);
  display.display();
 }

 }
  
  
  
  

     
  
  

	  // read the String message


    
  //  display.setCursor(5,25);
  //display.setTextSize(2);
  //display.print(String(95));
  //display.display();
 // display.clearDisplay();
	
 
//display.setCursor(5,25);
//  display.setTextSize(1);
 // display.print("RSSI:" + (String) Rssi);
 // display.display();  
	 // if the file is available, write to it:
 
  // if the file isn't open, pop up an error:
  
  
  }










