#include <Arduino.h>
//#include <Energia.h>
//#include <stdint.h>
#include <Geiger.h>


#include <U8g2lib.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  
//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


//OTA
//#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Kis-Szikora";
const char* password = "KisHangya";
//

#include <Startup.h>
#define buzzerPin 32


//Outputs

//Analog inputs
//#define POT     36
//Digital inputs

#define buttonPin1  25
#define buttonPin2  26
#define buttonPin3  27
#define buttonPin4  14

#define LEDpin 33

int LedPower = 0;

bool buttonPin1_state=1;
bool buttonPin2_state=1;
bool buttonPin3_state=1;
bool buttonPin4_state=1;

unsigned long currentMillis = millis();
unsigned long previousMillis_LoadingIcon = 0;
unsigned long previousMillis_DisplayUpdater = 0;

uint8_t BuzzerPin_PWM = 100;
bool buzzer_on = false;
uint8_t LEDPin_PWM = 100;
bool LED_on = false;

uint8_t IconState = 0;

char WifiStatusReturn [7][100] = 
	{
		"0 : WL_IDLE_STATUS: Wi-Fi is in process of changing between statuses",
		"1 : WL_NO_SSID_AVAIL: configured SSID cannot be reached",
		"2 : WL_SCAN_COMPLETED",
		"3 : WL_CONNECTED: successful connection is established",
		"4 : WL_CONNECT_FAILED: if password is incorrect",
		"5 : WL_CONNECTION_LOST:",
		"6 : WL_DISCONNECTED: module is not configured in station mode"
	};


void setLED(int LedPower);
void AliveAnimation();
void DisplayUpdater();
void ProcessSerialInput();





void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  uint8_t WifiTimeoutCounter = 0;
  while (WiFi.status() != WL_CONNECTED  && WifiTimeoutCounter != 2) {
    WifiTimeoutCounter++;
    delay(500);
  }
  if(WifiTimeoutCounter != 2){
    Serial.println(" Successfully connected to the Wifi!"); 
  	Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
  }else{
  	Serial.println("Timeout! Couldn't connect!"); 
  }
  Serial.printf("Connection status: %s\n", WifiStatusReturn[WiFi.status()]);

ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        //Write OTA flashing to the screen
        u8g2.clearBuffer(); // clear the internal memory
        u8g2.setFont(u8g2_font_helvR08_tr);
        u8g2.setCursor(10,15); 
        u8g2.print("FW Updating");
        u8g2.setCursor(10,28); 
        u8g2.print("Over the Air");
        u8g2.drawFrame(7,35,120,16);

        u8g2.setFontMode(1);  /* activate transparent font mode */
        u8g2.setDrawColor(1); /* color 1 for the box */

        u8g2.drawBox(7, 35, map((progress / (total / 100)),0,100,7,120), 16);
        u8g2.setFont(u8g2_font_ncenB14_tf);
        u8g2.setCursor(55,50); 
        u8g2.setDrawColor(2);
        //u8g2.print(Progress);u8g2.print("%");
        u8g2.print((progress / (total / 100)));u8g2.print("%");

        //u8g2.drawStr(60, 35, Progress);
        u8g2.sendBuffer();

    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  


  // put your setup code here, to run once:
    pinMode(LEDpin, OUTPUT);

    //analogWrite(pin, 0);  // turn the LED on (HIGH is the voltage level)
      pinMode(buzzerPin, OUTPUT);
      //pinMode(POT,    INPUT);
      pinMode(buttonPin1, INPUT_PULLUP);
      pinMode(buttonPin2, INPUT_PULLUP);
      pinMode(buttonPin3, INPUT_PULLUP);
      pinMode(buttonPin4, INPUT_PULLUP);

      u8g2.begin();  
      u8g2.enableUTF8Print();
  //u8g2.setDisplayRotation(U8G2_R2);
  u8g2.clearBuffer();
  u8g2.drawXBM( 0, 0, ClearSky_Startup_width, ClearSky_Startup_height, ClearSky_Startup_bits);
  u8g2.sendBuffer();
  play_tune(1, buzzerPin);
  delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:
/*
  delay(50);                      // wait for a second
  LedPower++;
  Serial.println(LedPower);
  if(LedPower==255){
    LedPower = 250;
  }*/
currentMillis = millis(); 
DisplayUpdater();
ProcessSerialInput();
ArduinoOTA.handle();

if(buttonPin1_state!=digitalRead(buttonPin1)){
  Serial.printf("Button1: %d\n", digitalRead(buttonPin1));
}
if(buttonPin2_state!=digitalRead(buttonPin2)){
  Serial.printf("Button2: %d\n", digitalRead(buttonPin2));
}
if(buttonPin3_state!=digitalRead(buttonPin3)){
  Serial.printf("Button3: %d\n", digitalRead(buttonPin3));
}
if(buttonPin4_state!=digitalRead(buttonPin4)){
  Serial.printf("Button4: %d\n", digitalRead(buttonPin4));
}

buttonPin1_state = digitalRead(buttonPin1);
buttonPin2_state = digitalRead(buttonPin2);
buttonPin3_state = digitalRead(buttonPin3);
buttonPin4_state = digitalRead(buttonPin4);





//Geiger(buzzerPin,90,150);

if(buzzer_on == true)
{
  analogWrite(buzzerPin, BuzzerPin_PWM);
}
else{
  analogWrite(buzzerPin, 0);
}

if(LED_on == true)
{
  analogWrite(LEDpin, LEDPin_PWM);
}
else{
  analogWrite(LEDpin, 0);
}




//Serial.printf("Pot: %d\n", analogRead(POT));
//This code only generates a delay of 500 milliseconds between the tone   



//analogWrite(buzzerPin,map(analogRead(POT),0,4100,0,255));
//Serial.printf("Pot: %d; AnalogW: %d\n", analogRead(POT), map(analogRead(POT),0,4100,0,255));

/*
tone(buzzerPin, 440); Serial.printf("buzzerPin: 1\n");
  delay(1000);

  tone(buzzerPin, 494); Serial.printf("buzzerPin: 2\n");
  delay(1000);

  tone(buzzerPin, 523); Serial.printf("buzzerPin: 3\n");
  delay(1000);
  noTone(buzzerPin);
  delay(1000);
*/


  /*
  for(int counter = 0;counter<3;counter++){
    for(int i = 0; i<100 ;i++){
      setLED(i);
      delay(5);
    }
    for(int i = 100; i>00 ;i--){
      delay(5);
      setLED(i);
    }
    delay(500);
  }

  for(int counter = 0;counter<4;counter++){
    setLED(100);
    delay(500);
    setLED(0);
    delay(500);
  }

 */ 
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setLED(int LedPower)
{
  int LedPowerAnalog = map(LedPower, 0, 100, 0, 255);
  analogWrite(LEDpin, LedPowerAnalog);  // turn the LED on (HIGH is the voltage level)
}


void AliveAnimation(){


  if (currentMillis - previousMillis_LoadingIcon >= 50) {
    // save the last time you blinked the LED
    previousMillis_LoadingIcon = currentMillis;
    IconState++;
    if(IconState == 8){
      IconState = 0;
    }
  }

  u8g2.setDrawColor(1);

  switch (IconState){
    case 0:
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      break;

    case 1:
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      break;
    case 2:
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      break;
    case 3:
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      u8g2.setDrawColor(0);
      break;



    case 4:
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      break;
    case 5:
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      break;
    case 6:
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.setDrawColor(1);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      break;
    case 7:
      u8g2.setDrawColor(0);
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
      
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
      u8g2.setDrawColor(1);
      break;
        
    }
    
  u8g2.setDrawColor(1);
}



void DisplayUpdater(){

  u8g2.clearBuffer(); // clear the internal memory
    AliveAnimation();

  
      u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
      u8g2.setCursor(0,7); 
      u8g2.print("Wifi connected");
      u8g2.setCursor(0,14); 
      u8g2.print("IP address: ");u8g2.print(WiFi.localIP());

    u8g2.sendBuffer();          // transfer internal memory to the display
  }






void ProcessSerialInput(){
// process any serial input
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    
    if (input >= 48 && input <= 57) //[0,9]
    {
      //TRANSMITPERIOD = 100 * (input-48);
      //if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 1000;
      //Serial.print("\nChanging delay to ");
      //Serial.print(TRANSMITPERIOD);
      //Serial.println("ms\n");
    }

    if (input == 'q') //buzzer on
    {
      buzzer_on = true;
      Serial.printf("Buzzer ON");
    }

    if (input == 'w') //buzzer off
    {
      buzzer_on = false;
      Serial.printf("Buzzer OFF");
    }
    if (input == 'e') //buzzer PWM++
    {
      BuzzerPin_PWM = BuzzerPin_PWM + 5;
      if(BuzzerPin_PWM>255){
        BuzzerPin_PWM = 0;
      }else if(BuzzerPin_PWM<0){
        BuzzerPin_PWM = 255;
      }
      Serial.printf("BuzzerPin_PWM: %d\n", BuzzerPin_PWM);
    }

    if (input == 'r') //buzzer PWM--
    {
      BuzzerPin_PWM = BuzzerPin_PWM - 5;
      if(BuzzerPin_PWM>255){
        BuzzerPin_PWM = 0;
      }else if(BuzzerPin_PWM<0){
        BuzzerPin_PWM = 255;
      }
      Serial.printf("BuzzerPin_PWM: %d\n", BuzzerPin_PWM);
    }

    //LED---------------------------
    if (input == 'a') //buzzer on
    {
      LED_on = true;
      Serial.printf("LED ON");
    }

    if (input == 's') //buzzer off
    {
      LED_on = false;
      Serial.printf("LED OFF");
    }
    if (input == 'd') //buzzer PWM++
    {
      LEDPin_PWM = LEDPin_PWM + 5;
      if(LEDPin_PWM>255){
        LEDPin_PWM = 0;
      }else if(LEDPin_PWM<0){
        LEDPin_PWM = 255;
      }
      Serial.printf("LEDPin_PWM: %d\n", LEDPin_PWM);
    }

    if (input == 'f') //buzzer PWM--
    {
      LEDPin_PWM = LEDPin_PWM - 5;
      if(LEDPin_PWM>255){
        LEDPin_PWM = 0;
      }else if(LEDPin_PWM<0){
        LEDPin_PWM = 255;
      }
      Serial.printf("LEDPin_PWM: %d\n", LEDPin_PWM);
    }
    //LED---------------------------
    if (input == 'y') //buzzer on
    {
      play_tune(1, buzzerPin);
      Serial.printf("play_tune");
    }

  }
}