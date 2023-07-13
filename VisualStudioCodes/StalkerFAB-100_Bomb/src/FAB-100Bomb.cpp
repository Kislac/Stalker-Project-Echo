//pio device monitor -p COM4 -b 115200
// Sample RFM69 sender/node sketch, with ACK and optional encryption, and Automatic Transmission Control
// Sends periodic messages of increasing length to gateway (id=1)
// It also looks for an onboard FLASH chip, if present
// **********************************************************************************
// Copyright Felix Rusu 2018, http://www.LowPowerLab.com/contact
// **********************************************************************************
// License
// **********************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 3 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE. See the GNU General Public        
// License for more details.                              
//                                                        
// Licence can be viewed at                               
// http://www.gnu.org/licenses/gpl-3.0.txt
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// **********************************************************************************
#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
//#include <SPIFlash.h>      //get it here: https://www.github.com/lowpowerlab/spiflash
#include <RFM69registers.h> // Include- block, needed for recognition of REG_BITRATEMSB / REG_BITRATELSB
#include <Arduino.h>
//#include <U8g2lib.h>
//#include <U8x8lib.h>
//#ifdef U8X8_HAVE_HW_I2C
//#include <Wire.h>
//#endif
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  
#include <Wire.h>

#include <EEPROM.h>

//OTA
//#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Xiaomi11T";
const char* password = "11112222";
//
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


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu;

#include <FastLED.h>
#define NUM_LEDS 12
uint8_t NUM_LEDS_Glow = 2;
#define DATA_PIN 33
CRGB leds[NUM_LEDS];


#include <Geiger.h>
#include <esp_task_wdt.h>
//3 seconds WDT
#define WDT_TIMEOUT 3
//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE ************
//*********************************************************************************************
// Address IDs are 10bit, meaning usable ID range is 1..1023
// Address 0 is special (broadcast), messages to address 0 are received by all *listening* nodes (ie. active RX mode)
// Gateway ID should be kept at ID=1 for simplicity, although this is not a hard constraint
//*********************************************************************************************
#define NODEID        9    // keep UNIQUE for each node on same network
#define NETWORKID     100  // keep IDENTICAL on all nodes that talk to each other
#define GATEWAYID     1    // "central" node

//*********************************************************************************************
// Frequency should be set to match the radio module hardware tuned frequency,
// otherwise if say a "433mhz" module is set to work at 915, it will work but very badly.
// Moteinos and RF modules from LowPowerLab are marked with a colored dot to help identify their tuned frequency band,
// see this link for details: https://lowpowerlab.com/guide/moteino/transceivers/
// The below examples are predefined "center" frequencies for the radio's tuned "ISM frequency band".
// You can always set the frequency anywhere in the "frequency band", ex. the 915mhz ISM band is 902..928mhz.
//*********************************************************************************************
#define FREQUENCY   RF69_433MHZ
//#define FREQUENCY   RF69_868MHZ
//#define FREQUENCY     RF69_915MHZ
//#define FREQUENCY_EXACT 916000000 // you may define an exact frequency/channel in Hz
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level (ATC_RSSI)
//#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI      -80
//*********************************************************************************************
#define SERIAL_BAUD   115200
#define RESET_PIN 4
#define Button_Blue 25 //Ѯ
#define Button_Yellow 26 // Ж
#define Button_Green 27 //θ
#define Button_Red 14 //Ω


#define buzzerPin 32
#define LEDpin 15 //not used currently. Not wired in

int TRANSMITPERIOD = 500; //transmit a packet to gateway so often (in ms)
char payload[] = "123456789";
char buff[20];
byte sendSize=1;
boolean requestACK = false;
//SPIFlash flash(SS_FLASHMEM, 0xEF30); //EF30 for 4mbit  Windbond chip (W25X40CL)

#ifdef ENABLE_ATC
  RFM69_ATC radio(5,16,true);
#else
  RFM69 radio(5,16,true);
  //RFM69 radio;
#endif

bool spy = false; //set to 'true' to sniff all packets on the same network

void ResetRadio()
{
  digitalWrite(RESET_PIN, HIGH);
  delay(50);
  digitalWrite(RESET_PIN, LOW);
  Serial.println("Radio reseted!");
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

void DisplayUpdater();
void AliveAnimation();
void ProcessSerialInput();
void RFM_Recive_msg();
void RFM_Send_msg();
void ButtonHandler();
void WatchDogFeeder();
void CodingHandler();
void BombPhaseHandler();
void AccelerationHandler();
void ConnectToWifi(uint16_t timeout);
void WifiHandle();

bool WifiStatus = false;
bool EnableWifi = false;
bool ConnectWifiFlag = false;
bool TurnOffWifiFlag = false;
unsigned long EnableWifi_timer = 0;

uint8_t fadeAmount = 5; 
uint8_t brightness = 5;
uint8_t ColorChangeNum = 0;
unsigned long previousMillis_LEDFadeTimer = 0;

unsigned long previousMillis_UnstableTimer = 0;
uint32_t UnstableTimer;

unsigned long ResetBombTimer = 0;
bool ResetBombTimerFlag =false;

unsigned long previousMillis_CodingChange = 0;
uint8_t CodeArray[10];
uint8_t DecryptedCodeArray[10];
bool ChangeCodeNow = true;

unsigned long previousMillis_DisplayUpdater = 0;
unsigned long previousMillis_LoadingIcon = 0;
uint8_t IconState;
uint32_t sentMSGCounter_Prev;
uint32_t sentMSGCounter;
uint32_t packetCount_Prev;
uint32_t packetCount = 0;
byte ackCount=0;
int CurrentRSSI = 0;

unsigned long previousMillis_WatchDogTimer = 0;
bool WatchDogEnable = true;

int8_t PowerLevel = 23;

char IncomingMsg_char[230];
int IncomingMsg_int[230];
// IncomingMSG_Lenght;
char SentMsg[64];
char IncomingMsg[64];


float AccelX_Prev;
float AccelY_Prev;
float AccelZ_Prev;
bool LightMotionDetected;
bool HardMotionDetected;


bool Button_Blue_state = 1;
bool Button_Yellow_state = 1;
bool Button_Green_state = 1;
bool Button_Red_state = 1;
bool Button_Blue_state_Prev = 1;
bool Button_Yellow_state_Prev = 1;
bool Button_Green_state_Prev = 1;
bool Button_Red_state_Prev = 1;

int ButtonInputs[10] = {0,0,0,0,0,0,0,0,0,0};
//                                  0   1   2    3
char ButtonInputsMeaning [4][3] = {"Ж","Ω","θ","Ѯ"};
bool Unarmed=false;


bool SendMsgFlag = true;
bool ReciveMsgFlag = true;


const uint8_t GraphResPixel = 5;
const uint8_t RSSI_LogSize = 25;//126/GraphResPixel;//20
int RSSI_Log[RSSI_LogSize];
uint8_t First_X;
uint8_t Second_X;
int8_t First_RSSI ;
int8_t Second_RSSI;
int8_t First_Y;
int8_t Second_Y;


int8_t BuzzerIntensity= 80;
uint8_t BuzzerVolume = 100;
//uint8_t BuzzerPWM = 255;
bool enableBuzzer= false;
unsigned long previousMillis_Buzzer = 0;
int delta = 2000;

bool enableGeiger = false;

uint8_t BombPhase = 1;

int8_t BITRATE_Counter = 6;
uint8_t BITRATE[22][2] ={
{	RF_BITRATELSB_1200	,	RF_BITRATEMSB_1200	}	,
{	RF_BITRATELSB_2400	,	RF_BITRATEMSB_2400	}	,
{	RF_BITRATELSB_4800	,	RF_BITRATEMSB_4800	}	,
{	RF_BITRATELSB_9600	,	RF_BITRATEMSB_9600	}	,
{	RF_BITRATELSB_12500	,	RF_BITRATEMSB_12500	}	,
{	RF_BITRATELSB_19200	,	RF_BITRATEMSB_19200	}	,
{	RF_BITRATELSB_25000	,	RF_BITRATEMSB_25000	}	,
{	RF_BITRATELSB_32768	,	RF_BITRATEMSB_32768	}	,
{	RF_BITRATELSB_34482	,	RF_BITRATEMSB_34482	}	,
{	RF_BITRATELSB_38323	,	RF_BITRATEMSB_38323	}	,
{	RF_BITRATELSB_38400	,	RF_BITRATEMSB_38400	}	,
{	RF_BITRATELSB_50000	,	RF_BITRATEMSB_50000	}	,
{	RF_BITRATELSB_55555	,	RF_BITRATEMSB_55555	}	,
{	RF_BITRATELSB_57600	,	RF_BITRATEMSB_57600	}	,
{	RF_BITRATELSB_76800	,	RF_BITRATEMSB_76800	}	,
{	RF_BITRATELSB_100000	,	RF_BITRATEMSB_100000	}	,
{	RF_BITRATELSB_115200	,	RF_BITRATEMSB_115200	}	,
{	RF_BITRATELSB_150000	,	RF_BITRATEMSB_150000	}	,
{	RF_BITRATELSB_153600	,	RF_BITRATEMSB_153600	}	,
{	RF_BITRATELSB_200000	,	RF_BITRATEMSB_200000	}	,
{	RF_BITRATELSB_250000	,	RF_BITRATEMSB_250000	}	,
{	RF_BITRATELSB_300000	,	RF_BITRATEMSB_300000	}	
};

int BITRATE_Meaning[22] =
  { 
  1200	,
  2400	,
  4800	,
  9600	,
  12500	,
  19200	,
  25000	,
  32768	,
  34482	,
  38323	,
  38400	,
  50000	,
  55555	,
  57600	,
  76800	,
  100000	,
  115200	,
  150000	,
  153600	,
  200000	,
  250000	,
  300000
  };

//void AliveAnimation();
//void DisplayUpdater();
//void ProcessSerialInput();
//void RFM_Recive_msg();
//void RFM_Send_msg();

void setup() {
  //delay(2000);
    for(int i = 0; i < RSSI_LogSize;  i++ )//clear IncomingMsg[] array
      RSSI_Log[i] = -100;

      pinMode(Button_Blue, INPUT_PULLUP);
      pinMode(Button_Yellow, INPUT_PULLUP);
      pinMode(Button_Green, INPUT_PULLUP);
      pinMode(Button_Red, INPUT_PULLUP);

      pinMode(LEDpin, OUTPUT);
      pinMode(buzzerPin, OUTPUT);

//setup FastLED
FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);



 //setupt motion detection
// Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

   sensors_event_t a, g, temp;
   mpu.getEvent(&a, &g, &temp);
   AccelX_Prev = a.acceleration.x;
   AccelY_Prev = a.acceleration.y;
   AccelZ_Prev = a.acceleration.z;

  Serial.begin(SERIAL_BAUD);
  ResetRadio();
  radio.initialize(FREQUENCY,NODEID,NETWORKID);

radio.writeReg(REG_BITRATEMSB, RF_BITRATEMSB_25000); // setup- function, after radio.initialize(...)
radio.writeReg(REG_BITRATELSB, RF_BITRATELSB_25000);   // setup- function, after radio.initialize(...)


#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif

#ifdef ENCRYPTKEY
  //radio.encrypt(ENCRYPTKEY);
  radio.encrypt(NULL);
#endif

#ifdef FREQUENCY_EXACT
  radio.setFrequency(FREQUENCY_EXACT); //set frequency to some custom frequency
#endif
  
//Auto Transmission Control - dials down transmit power to save battery (-100 is the noise floor, -90 is still pretty good)
//For indoor nodes that are pretty static and at pretty stable temperatures (like a MotionMote) -90dBm is quite safe
//For more variable nodes that can expect to move or experience larger temp drifts a lower margin like -70 to -80 would probably be better
//Always test your ATC mote in the edge cases in your own environment to ensure ATC will perform as you expect
#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
#endif

  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);


#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)\n");
#endif

//u8g2.begin();  
//u8g2.enableUTF8Print();
//u8g2.setDisplayRotation(U8G2_R2);

ConnectToWifi(2000);
ArduinoOTA
    .onStart([]() {
      esp_task_wdt_init(6000, true); //enable panic so ESP32 restarts
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
  WiFi.mode(WIFI_OFF);WifiStatus = false;
  Serial.printf("WiFi.mode(WIFI_OFF);");


esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
esp_task_wdt_add(NULL); //add current thread to WDT watch

EEPROM.begin(255);  //Initialize EEPROM
BombPhase		= EEPROM.read(0);
Serial.printf("BombPhase: %d\n", BombPhase);
if(BombPhase == 255){
  BombPhase = 1;
}
Serial.printf("BombPhase: %d\n", BombPhase);

FastLED.clear();  // clear all pixel data
FastLED.show();

}



long lastPeriod = 0;
void loop() {
  //DisplayUpdater();
  CodingHandler();
  ProcessSerialInput();
  if(ReciveMsgFlag == true){RFM_Recive_msg();}
  if(SendMsgFlag == true){RFM_Send_msg();}
  ButtonHandler();
  //WifiHandle();
  //delay(1);

  //Geiger(buzzerPin,map(CurrentRSSI,-100,-40,0,100));
 // Geiger(buzzerPin,BuzzerIntensity);
 WatchDogFeeder();
 BombPhaseHandler();
 AccelerationHandler();
 ArduinoOTA.handle();

if (enableGeiger== true)
{
  Geiger(buzzerPin,BuzzerIntensity);
}



}


void ProcessSerialInput(){

//process any serial input
//Used diagnostic characters:
//1-9 Change Sending Period
//R - read all register
//E   Enyript ON
//e   Enyript Off
//i   Information 
//t   read temp
//y   toggle receiving frame
//x   toggle sending frame
//c   powerlevel--
//v   powerlevel+
//b   bitrate--
//n   bitrate++
//p   reset radio
//k   BuzzerIntensity++
//l   BuzzerIntensity--
//h   GeigerON
//j   GeigerOff
//g   Delay 5 sec (to test WDT)
//z   BuzzerOn
//u   BuzzerOff
//.   BuzzerPWM--
//-   BuzzerPWM++
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    if (input >= 48 && input <= 57) //[0,9]
    {
      TRANSMITPERIOD = 100 * (input-48);
      if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 1000;
      Serial.print("\nChanging delay to ");
      Serial.print(TRANSMITPERIOD);
      Serial.println("ms\n");
    }

    if (input == 'r') //d=dump register values
      radio.readAllRegs();

    if (input == 'E') //E=enable encryption
      radio.encrypt(ENCRYPTKEY);
    if (input == 'e') //e=disable encryption
      radio.encrypt(null);
if (input == 'i') // print all available setup infos
    {
      Serial.println();
      Serial.println("***************************************************************************");
      //return name of file and compile date/time
      Serial.println(__FILE__ " " __DATE__ " " __TIME__);
      
      //Serial.print("Slave Select Pin: ");
      //Serial.println(radio.getSlaveSelectPin());
      //Serial.print("Interrupt Pin: ");
      //Serial.println(radio.getInterruptPin());
      //Serial.print("Interrupt Number: ");
      //Serial.println(radio.getInterruptNumber());
      //Serial.print("Is RFM69HW: ");
      //Serial.println(radio.getHighPower());
      Serial.print("Power Level: ");
      Serial.println(radio.getPowerLevel());
      Serial.print("Frequency: ");
      Serial.println(radio.getFrequency());
      //Serial.print("NODEID: ");
      //Serial.println(radio.getAdress());
      //Serial.print("NETWORKID: ");
      //Serial.println(radio.getNetwork());

      //Serial.print("Test Pin (LED on STM32F103C Bluepill): ");
      //Serial.println(radio.getTestPin());
      Serial.println("***************************************************************************");
      Serial.println();
    }
    if (input == 't')
    {
      byte temperature =  radio.readTemperature(-1); // -1 = user cal factor, adjust for correct ambient
      byte fTemp = 1.8 * temperature + 32; // 9/5=1.8
      Serial.print( "Radio Temp is ");
      Serial.print(temperature);
      Serial.print("C, ");
      Serial.print(fTemp); //converting to F loses some resolution, obvious when C is on edge between 2 values (ie 26C=78F, 27C=80F)
      Serial.println('F');
    } 
    if (input == 'y')
    {
      ReciveMsgFlag = !ReciveMsgFlag;
      Serial.printf("ReciveMsgFlag: %d\n",ReciveMsgFlag);
    }
    if (input == 'x')
    {
      SendMsgFlag = !SendMsgFlag;
      Serial.printf("SendMsgFlag: %d\n",SendMsgFlag);
    }  
  
  if (input == 'c')
    {
      PowerLevel++;
      if (PowerLevel>23) PowerLevel = 23;
      radio.setPowerLevel(PowerLevel);
      Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
    }
    if (input == 'v')
    {
      PowerLevel--;
      if (PowerLevel<0) PowerLevel = 0;
      radio.setPowerLevel(PowerLevel);
      Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
    } 
    if (input == 'b')
    {
      BITRATE_Counter--;
      if (BITRATE_Counter<0) {BITRATE_Counter = 0;}
      radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
      radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
      Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
    }
    if (input == 'n')
    {
      BITRATE_Counter++;
      if (BITRATE_Counter>22) {BITRATE_Counter = 22;}
      radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
      radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
      Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
    }
    if (input == 'p')
    {
      ResetRadio(); 
    }
    if (input == 'k')
    {
      WiFi.mode(WIFI_STA);
      WifiStatus = true;
      Serial.printf("WiFi.mode(WIFI_STA);");
    }
    if (input == 'l')
    {
      Serial.printf("Connection status: %s\n", WifiStatusReturn[WiFi.status()]);
    }
    //BuzzerVolume
    if (input == 'h')
    {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      WifiStatus = false;
      Serial.printf("WiFi.mode(WIFI_OFF);");
    }
    if (input == 'j')
    {
      ConnectToWifi(5000);
    }

    if (input == 'z')
    {
      enableBuzzer = true;
      //BuzzerPWM = 255;
      //analogWrite(buzzerPin,BuzzerPWM);
      //Serial.printf("BuzzerPWM: %d\n",BuzzerPWM);
    }
    if (input == 'u')
    {
      enableBuzzer = false;
      digitalWrite(buzzerPin,0);
      //BuzzerPWM = 0;
      //analogWrite(buzzerPin,BuzzerPWM);
      //Serial.printf("BuzzerPWM: %d\n",BuzzerPWM);
    }
    if (input == '.')
    {
        NUM_LEDS_Glow--;
        if(NUM_LEDS_Glow<=0){NUM_LEDS_Glow=0;}
        else if(NUM_LEDS_Glow>5){NUM_LEDS_Glow=5;}
        Serial.printf("NUM_LEDS_Glow: %d\n",NUM_LEDS_Glow);

        FastLED.clear();  // clear all pixel data
        FastLED.show();
    }
    if (input == '-')
    {
        NUM_LEDS_Glow++;
        if(NUM_LEDS_Glow<=0){NUM_LEDS_Glow=0;}
        else if(NUM_LEDS_Glow>5){NUM_LEDS_Glow=5;}
        Serial.printf("NUM_LEDS_Glow: %d\n",NUM_LEDS_Glow);

        
        FastLED.clear();  // clear all pixel data
        FastLED.show();

    }
    if (input == 'q')
    {
      BombPhase = 1;
      EEPROM.write(0, 1);
      EEPROM.commit();
      Serial.printf("Reseting to default values now!\n");
      ResetBombTimerFlag = false;
      for(int b=0;b<2;b++){
      digitalWrite(buzzerPin,1);
      for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
      digitalWrite(buzzerPin,0);
      for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
      }
        
    }
    
    if(input == 'f'){
        BombPhase--;
        if(BombPhase<0){BombPhase=0;}
        else if(BombPhase>5){BombPhase=5;}
        EEPROM.write(0, BombPhase);
        EEPROM.commit();
        Serial.printf("BombPhase: %d\n",BombPhase);
    }
    if(input == 'g'){
         BombPhase++;
         if(BombPhase<0){BombPhase=0;}
        else if(BombPhase>6){BombPhase=6;}
        EEPROM.write(0, BombPhase);
        EEPROM.commit();
        Serial.printf("BombPhase: %d\n",BombPhase);
    }
    if(input == 'a'){
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow)
        {leds[i] = CRGB::Red;}
        FastLED.show();  
    }
    if(input == 's'){
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow)
        {leds[i] = CRGB::Green;}
        FastLED.show();  
    }
    if(input == 'd'){
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow)
        {leds[i] = CRGB::Blue;}
        FastLED.show();  
    }
    

  }

}

void RFM_Recive_msg(){
if (radio.receiveDone())
  {
    Serial.print("#[");
    Serial.print(++packetCount);
    Serial.print(']');
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    if (spy) Serial.print("to [");Serial.print(radio.TARGETID, DEC);Serial.print("] ");

    for(int b = 0; b < sizeof(IncomingMsg_char);  ++b )//clear IncomingMsg[] array
        IncomingMsg_char[b] = (char)0;

    for (byte i = 0; i < radio.DATALEN; i++){
      Serial.print((char)radio.DATA[i]);
      IncomingMsg_char[i] = (char)radio.DATA[i];
      IncomingMsg_int[i] = radio.DATA[i];
    }
    CurrentRSSI = radio.readRSSI();
    Serial.print("   [RX_RSSI:");Serial.print(CurrentRSSI);Serial.print("]");
    packetCount_Prev = packetCount; 


if(strcmp(IncomingMsg_char, "Turn on WiFi!") == 0){
    Serial.printf("Turn on Wifi\n");
    ConnectToWifi(5000);


}else if(strcmp(IncomingMsg_char, "Turn off WiFi!") == 0){
WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      WifiStatus = false;
      Serial.printf("WiFi.mode(WIFI_OFF);");
}else if(strcmp(IncomingMsg_char, "Reset Bomb!") == 0){
  BombPhase = 1;
  NUM_LEDS_Glow = 1;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
  ESP.restart();
}else if(strcmp(IncomingMsg_char, "Bomb Phase:0") == 0){
  BombPhase = 0;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}else if(strcmp(IncomingMsg_char, "Bomb Phase:1") == 0){
  BombPhase = 1;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}else if(strcmp(IncomingMsg_char, "Bomb Phase:2") == 0){
  BombPhase = 2;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}else if(strcmp(IncomingMsg_char, "Bomb Phase:3") == 0){
  BombPhase = 3;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}else if(strcmp(IncomingMsg_char, "Bomb Phase:4") == 0){
  BombPhase = 4;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}else if(strcmp(IncomingMsg_char, "Bomb Phase:5") == 0){
  BombPhase = 5;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}else if(strcmp(IncomingMsg_char, "Bomb Phase:6") == 0){
  BombPhase = 6;
  EEPROM.write(0, BombPhase);EEPROM.commit();
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
}




//-----------ACK------------------
//    if (radio.ACKRequested())
//    {
//      byte theNodeID = radio.SENDERID;
//      radio.sendACK();
//      Serial.print(" - ACK sent.");
//
//      // When a node requests an ACK, respond to the ACK
//      // and also send a packet requesting an ACK (every 3rd one only)
//      // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
//      if (ackCount++%3==0)
//      {
//        Serial.print(" Pinging node ");
//        Serial.print(theNodeID);
//        Serial.print(" - ACK...");
//        delay(3); //need this when sending right after reception .. ?
//        if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
//          Serial.print("ok!");
//        else Serial.print("nothing");
//      }
//    }
//


    Serial.println();
    //Blink(LED_BUILTIN,3);
    Blink(LEDpin,1);
  }
}

void RFM_Send_msg(){
//check for any received packets
  /*if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);
    Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    //Blink(LED_BUILTIN,3);
    Blink(LED_BUILTIN,1);
    Serial.println(33);
  }*/

  int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    lastPeriod=currPeriod;

    for(int b = 0; b < sizeof(SentMsg);  ++b )//clear SentMsg[]array
        SentMsg[b] = (char)0;

    //send FLASH id
    if(sendSize==0)
    {

      //byte buffLen=strlen(buff);
      //if (radio.sendWithRetry(GATEWAYID, buff, buffLen))
      //  Serial.print(" ok!");
      //else Serial.print(" nothing...");
      //sendSize = (sendSize + 1) % 31;
    }
    else
    {
      Serial.print("Sending[");
      Serial.print(sendSize);
      Serial.print("]: ");
      for(byte i = 0; i < sendSize; i++){
        Serial.print((char)payload[i]);
        SentMsg[i] = (char)payload[i];
      }
        
      TRANSMITPERIOD = 500;
      sentMSGCounter++;
      sendSize++;
      if(sendSize>=10){
        sendSize=10;
      }
      
      
      //radio.send(GATEWAYID, payload, sendSize);
      radio.send(GATEWAYID, payload, sendSize);
      //if (radio.sendWithRetry(GATEWAYID, payload, sendSize))
      // Serial.print(" ok!");
      //else Serial.print(" nothing...");
    }
    //sendSize = (sendSize + 1) % 31;
    //sendSize = (sendSize + 1) % 4;
    Serial.println();
    Blink(LED_BUILTIN,1);
    //Blink(33,3);
  }
}



void ButtonHandler(){

  Button_Blue_state = digitalRead(Button_Blue);
  Button_Yellow_state= digitalRead(Button_Yellow);
  Button_Green_state = digitalRead(Button_Green);
  Button_Red_state= digitalRead(Button_Red);




  if(Button_Blue_state_Prev!=Button_Blue_state){
    //Serial.printf("Button-B: %d\n", Button_Blue_state);
    if(Button_Blue_state==0){
      for(int i=0; i<9;i++){
        ButtonInputs[i]= ButtonInputs[i+1];
        Serial.printf("%d", ButtonInputs[i]);
      }
      ButtonInputs[9]=3;
      Serial.printf("%d\n", ButtonInputs[9]);


      //code
      //PowerLevel++;
      //if (PowerLevel>23) PowerLevel = 23;
      //radio.setPowerLevel(PowerLevel);
      //ChangeCodeNow = true;
    }
  }
  if(Button_Yellow_state_Prev!=Button_Yellow_state){
    //Serial.printf("Button-Y: %d\n", Button_Yellow_state);
    if(Button_Yellow_state==0){
      for(int i=0; i<9;i++){
        ButtonInputs[i]= ButtonInputs[i+1];
        Serial.printf("%d", ButtonInputs[i]);
      }
      ButtonInputs[9]=0;
      Serial.printf("%d\n", ButtonInputs[9]);
      //code
      //PowerLevel--;
      //if (PowerLevel>23) PowerLevel = 23;
      //radio.setPowerLevel(PowerLevel);
    }
  }
  if(Button_Green_state_Prev!=Button_Green_state){
    //Serial.printf("Button-G: %d\n", Button_Green_state);
    if(Button_Green_state==0){
      for(int i=0; i<9;i++){
        ButtonInputs[i]= ButtonInputs[i+1];
        Serial.printf("%d", ButtonInputs[i]);
      }
      ButtonInputs[9]=2;
      Serial.printf("%d\n", ButtonInputs[9]);
      ////code
      //BITRATE_Counter++;
      //if (BITRATE_Counter>22) {BITRATE_Counter = 22;}
      //radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
      //radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
    }
  }
  if(Button_Red_state_Prev!=Button_Red_state){
    //Serial.printf("Button_R: %d\n", Button_Red_state);
    if(Button_Red_state==0){
      for(int i=0; i<9;i++){
        ButtonInputs[i]= ButtonInputs[i+1];
        Serial.printf("%d", ButtonInputs[i]);
      }
      ButtonInputs[9]=1;
      Serial.printf("%d\n", ButtonInputs[9]);
      ////code
      //BITRATE_Counter--;
      //if (BITRATE_Counter>22) {BITRATE_Counter = 22;}
      //radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
      //radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
    }
  }

  if(
    ButtonInputs[0]==DecryptedCodeArray[0] &&
    ButtonInputs[1]==DecryptedCodeArray[1] &&
    ButtonInputs[2]==DecryptedCodeArray[2] &&
    ButtonInputs[3]==DecryptedCodeArray[3] &&
    ButtonInputs[4]==DecryptedCodeArray[4] &&
    ButtonInputs[5]==DecryptedCodeArray[5] &&
    ButtonInputs[6]==DecryptedCodeArray[6] &&
    ButtonInputs[7]==DecryptedCodeArray[7] &&
    ButtonInputs[8]==DecryptedCodeArray[8] &&
    ButtonInputs[9]==DecryptedCodeArray[9] &&
    Unarmed == false
  ){
    Unarmed = true;
    Serial.printf("Unarmed!\n");
  }


if(//start reseting to default values
  Button_Blue_state == 0 &&
  Button_Yellow_state == 0 &&
  ResetBombTimerFlag == false
){
  ResetBombTimer = millis();
  ResetBombTimerFlag = true;
  Serial.printf("Reset Started!\n");
}

if(// iff button relesed cancel reseting
  (Button_Blue_state == 1 ||
  Button_Yellow_state == 1) &&
  ResetBombTimerFlag == true
){
  ResetBombTimerFlag = false;
  Serial.printf("Cancel reseting!\n");
  }

if(millis()-ResetBombTimer>5000 &&
  Button_Blue_state == 0 &&
  Button_Yellow_state == 0 &&
  ResetBombTimerFlag == true
){
  BombPhase = 1;
  EEPROM.write(0, 1);
  EEPROM.commit();
  Serial.printf("Reseting to default values now!\n");
  ResetBombTimerFlag = false;
  for(int b=0;b<2;b++){
        digitalWrite(buzzerPin,1);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Black;}FastLED.show();delay(250);
  }
} 

  Button_Blue_state_Prev = Button_Blue_state;
  Button_Yellow_state_Prev = Button_Yellow_state;
  Button_Green_state_Prev = Button_Green_state;
  Button_Red_state_Prev = Button_Red_state;
}

void WatchDogFeeder(){
  if (millis() - previousMillis_WatchDogTimer >= 2000) {
    previousMillis_WatchDogTimer = millis();
    esp_task_wdt_reset();
  }
}

void CodingHandler(){

  if (millis() - previousMillis_CodingChange >= 30000) {
      previousMillis_CodingChange = millis();
      ChangeCodeNow = true;
  }

  if (ChangeCodeNow == true){
    Serial.printf("Code changed now: " );
    sendSize = 1;
    for (int i = 0; i<10;i++){
      CodeArray[i]=(rand() % 10);
      Serial.printf("%d", CodeArray[i]);
      DecryptedCodeArray[i] = (((CodeArray[i]^6)<<2)%5)%4;
      payload[i] = CodeArray[i]+48;
      delay(10);
    }

    Serial.printf("\n" );

    Serial.printf("Decrypted code: " );
    for (int i = 0; i<10;i++){
      Serial.printf("%d", DecryptedCodeArray[i]);
    }
    Serial.printf("\n" );
    ChangeCodeNow = false;



  
  }


}


void BombPhaseHandler(){
  

  switch(BombPhase){
    case 1:
      Geiger(buzzerPin,80);
      if (millis() - previousMillis_LEDFadeTimer >= 50) {
        previousMillis_LEDFadeTimer = millis();
        for(int i = 0; i < NUM_LEDS; i=i+NUM_LEDS_Glow ){
          switch (ColorChangeNum){
            case 0:leds[i] = CRGB::Green;break;
            case 1:leds[i] = CRGB::Blue;break;
            case 2:leds[i] = CRGB::Red;break;
            case 3:leds[i] = CRGB::Yellow;break;
            case 4:leds[i] = CRGB::Purple;break;
          }
          leds[i].fadeLightBy(brightness);
        }
        FastLED.show();
        brightness = brightness + fadeAmount;
        // reverse the direction of the fading at the ends of the fade:
        if(brightness == 0 || brightness == 255){
          fadeAmount = -fadeAmount ;
        }  
        if (brightness == 255){
          ColorChangeNum++;
        }
        if (ColorChangeNum>=2){
          ColorChangeNum=0;
        }
        //Serial.printf("brightness: %d; ColorChangeNum:%d\n",brightness,ColorChangeNum );
      }
      if(LightMotionDetected == true){
        Serial.printf("Motion Detected. Bomb is Armed. Ticking Started!\n" );
        Serial.printf("----SWITCHING to Phase2----\n");
        LightMotionDetected = false;
        HardMotionDetected = false;
        BombPhase = 2;
        EEPROM.write(0, BombPhase);EEPROM.commit();
      }

      break;
    case 2:
      //delta = millis() - previousMillis_Buzzer;

      for(int b=0;b<10;b++){
        digitalWrite(buzzerPin,1);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Green;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,1);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Blue;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);
        esp_task_wdt_reset();   
      }
      BombPhase = 3;
      EEPROM.write(0, BombPhase);EEPROM.commit();
      Serial.printf("----SWITCHING to Phase3----\n");
      

      break;
    case 3:
        Geiger(buzzerPin,90);

        if(LightMotionDetected == true){
          Serial.printf("Motion Detected. Bomb is Armed. Ticking Started!\n" );
          LightMotionDetected = false;
          HardMotionDetected = false;
          BombPhase = 2;
          EEPROM.write(0, BombPhase);EEPROM.commit();
          Serial.printf("----SWITCHING to Phase2----\n");
        }
        if(Unarmed == true){
          BombPhase = 4;
          EEPROM.write(0, BombPhase);EEPROM.commit();
          Serial.printf("----SWITCHING to Phase4----\n");
          for(int i=0;i<40;i++){
            digitalWrite(buzzerPin,1);delay(50);
            digitalWrite(buzzerPin,0);delay(50);
          }
          digitalWrite(buzzerPin,1);delay(3000);
          digitalWrite(buzzerPin,0);
        }



        delta = millis() - previousMillis_Buzzer;
        if (delta >= 0 && delta <150){
          digitalWrite(buzzerPin,1);//delay(150);
          for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){
            leds[i] = CRGB::Green;
          }
          FastLED.show();
        }
        else if (delta >= 150 && delta <330){
          digitalWrite(buzzerPin,0);//delay(180);
          for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){
            leds[i] = CRGB::Green;
          }
          FastLED.show();
        }
        else if(delta >= 330 && delta <390){
          digitalWrite(buzzerPin,1);//delay(60);
          for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){
            leds[i] = CRGB::Red;
          }
          FastLED.show();
        }
        else if (delta >= 390 && delta <990){
          digitalWrite(buzzerPin,0);//delay(600);
          for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){
            leds[i] = CRGB::Red;
          }
        FastLED.show();
        }
        else if(delta>1000){
          previousMillis_Buzzer = millis();
        }
      break;

    case 4: //Unarmed!
      Geiger(buzzerPin,70);
      if (millis() - previousMillis_LEDFadeTimer >= 25) {
        previousMillis_LEDFadeTimer = millis();
        for(int i = 0; i < NUM_LEDS; i++ ){
          //switch (ColorChangeNum){
          //  case 0:leds[i] = CRGB::Green;break;
          //  case 1:leds[i] = CRGB::Blue;break;
          //  case 2:leds[i] = CRGB::Red;break;
          //  case 3:leds[i] = CRGB::Yellow;break;
          //  case 4:leds[i] = CRGB::Purple;break;
          //}
          leds[i] = CRGB::Yellow;
          leds[i].fadeLightBy(brightness);
        }
        FastLED.show();
        brightness = brightness + fadeAmount;
        // reverse the direction of the fading at the ends of the fade:
        if(brightness == 0 || brightness == 255){
          fadeAmount = -fadeAmount ;
        }  
        //if (brightness == 255){
        //  ColorChangeNum++;
        //}
        //if (ColorChangeNum==2){
        //  ColorChangeNum=0;
        //}

        if(HardMotionDetected == true){
        Serial.printf("Motion Detected. Bomb is Unstable. Stabilizing timer started!\n" );
        LightMotionDetected = false;
        HardMotionDetected = false;
        BombPhase = 5;
        EEPROM.write(0, BombPhase);EEPROM.commit();
        Serial.printf("----SWITCHING to Phase5----\n");
        }
      }
      break;
    case 5:
      Geiger(buzzerPin,95);
      for(int b=0;b<10;b++){
        digitalWrite(buzzerPin,1);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Red;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Green;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,1);
        for(int i= 0; i<NUM_LEDS;i=i+NUM_LEDS_Glow){leds[i] = CRGB::Blue;}FastLED.show();delay(250);
        digitalWrite(buzzerPin,0);
        esp_task_wdt_reset();  
      }
      BombPhase = 6;
      EEPROM.write(0, BombPhase);EEPROM.commit();
      Serial.printf("----SWITCHING to Phase6----\n");
      previousMillis_UnstableTimer = millis();
      UnstableTimer = 0.5*60*1000;//-->1 hour 
      ColorChangeNum = 0;
      break;
    case 6:
      Geiger(buzzerPin,95);
      if (millis() - previousMillis_LEDFadeTimer >= 1000) {
        previousMillis_LEDFadeTimer = millis();
        for(int i = 0; i < NUM_LEDS; i++ ){
          switch (ColorChangeNum){
            
            case 0:leds[i] = CRGB::Blue;break;
            case 1:leds[i] = CRGB::Red;break;
            
            //case 2:leds[i] = CRGB::Green;break;
          }
          //leds[i].fadeLightBy(0);
        }
        FastLED.show();
        ColorChangeNum++;
        if (ColorChangeNum>=2){
          ColorChangeNum=0;
        }
      }
      if (millis() - previousMillis_UnstableTimer >= UnstableTimer){
        BombPhase = 4;
        EEPROM.write(0, BombPhase);EEPROM.commit();
        Serial.printf("Timer expired. Bomb is Stable!\n" );
        Serial.printf("----SWITCHING to Phase4----\n");
      }
      if(HardMotionDetected == true){
        Serial.printf("Motion Detected. Bomb is Unstable. Stabilizing timer started!\n" );
        Serial.printf("----SWITCHING to Phase5----\n");
        LightMotionDetected = false;
        HardMotionDetected = false;
        EEPROM.write(0, BombPhase);EEPROM.commit();
        BombPhase = 5;
        }
      //previousMillis_UnstableTimer

      break;
  
  }

}

void AccelerationHandler(){
sensors_event_t a, g, temp;

  if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    
mpu.getEvent(&a, &g, &temp);

float deltaX = abs(AccelX_Prev - a.acceleration.x);
float deltaY = abs(AccelY_Prev - a.acceleration.y);
float deltaZ = abs(AccelZ_Prev - a.acceleration.z);
float MaxAcceleration = max(deltaX, deltaY);
MaxAcceleration = max(MaxAcceleration, deltaZ);
//Serial.printf("DeltaAcceleration: %f\n", MaxAcceleration);

if(MaxAcceleration >= 5){
  LightMotionDetected = true;
  Serial.printf("LightMotionDetected: %f\n", MaxAcceleration);
}
if(MaxAcceleration >= 8){
  HardMotionDetected = true;
  Serial.printf("HardMotionDetected: %f\n", MaxAcceleration);
}


AccelX_Prev = a.acceleration.x;
AccelY_Prev = a.acceleration.y;
AccelZ_Prev = a.acceleration.z;

  }
}


void ConnectToWifi(uint16_t timeout){
    esp_task_wdt_init(6000, true); //enable panic so ESP32 restarts
    WiFi.mode(WIFI_STA);
    //Serial.println("WiFi.mode(WIFI_STA);"); 
    WifiStatus = true;
    WiFi.disconnect();
    //Serial.println("WiFi.disconnect();"); 
    WiFi.begin(ssid, password);
    //Serial.println("WiFi.begin(ssid, password);"); 
    uint8_t WifiTimeoutCounter = 0;
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis()-start < timeout){
      Serial.printf("Time: %d\r", millis()-start);
    }
//
    if(millis()-start <= timeout){
      Serial.printf("Successfully connected to the Wifi! Connection time: %dms\n", millis()-start);
      //Serial.printf("Local IP address: %s\n", WiFi.localIP());
      Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
      
    }else{
      Serial.println("Timeout! Couldn't connect!");

    }
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
}

//void WifiHandle(){
//  if(EnableWifi==true){
//    EnableWifi_timer = millis();
//    TurnOffWifiFlag = true;
//    TRANSMITPERIOD = 1000;
//  } 
//  if(EnableWifi==true && ConnectWifiFlag == false){
//    ConnectToWifi(5000);
//    ConnectWifiFlag = true;
//    //payload[i]=
//    String  IP = WiFi.localIP().toString();
//    int str_len = IP.length(); 
//    //Serial.printf("String: %s, len: %d\n", IP, str_len);
//    sendSize = str_len;
//    //TRANSMITPERIOD = 1000;
//
//    IP.toCharArray(payload, str_len);
//
//    Serial.print("asdasdSending[");
//      Serial.print(sendSize);
//      Serial.print("]: ");
//      for(byte i = 0; i < sendSize; i++){
//        Serial.print((char)payload[i]);
//        SentMsg[i] = (char)payload[i];
//      }
//    Serial.println();
//    TRANSMITPERIOD = 1000;
//    
//
// 
//
//    
//
//    //strcpy(payload, WiFi.localIP().toString());
//  } 
//
//
//
//  if (millis() - EnableWifi_timer >= 10000 && TurnOffWifiFlag == true) {
//        EnableWifi =false;
//        WiFi.disconnect();
//        WiFi.mode(WIFI_OFF);
//        WifiStatus = false;
//        Serial.printf("Turn OFF WiFi!\n"); 
//        TurnOffWifiFlag = false;
//        ConnectWifiFlag = false;
//  }
//  EnableWifi =false;
//}