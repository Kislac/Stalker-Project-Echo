#define ClearSky_Startup_width 128
#define ClearSky_Startup_height 64
static unsigned char ClearSky_Startup_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xfc, 0xff, 0xf3,
   0xc7, 0xff, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0xf8, 0x01, 0xc3, 0xc3, 0x80, 0x1f, 0x7e, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x81, 0xc3, 0xc3, 0x83, 0x0f, 0xff,
   0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xe3, 0xe0, 0xc3,
   0xc3, 0x07, 0x8f, 0x7f, 0x00, 0x3c, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0x87, 0xe0, 0x83, 0xc3, 0x0f, 0xe2, 0xff, 0x00, 0xff, 0x71, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0xf0, 0x87, 0xe3, 0x0f, 0xe0, 0x7f,
   0xc0, 0xff, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xf0, 0xc7,
   0xe1, 0x19, 0xe0, 0xff, 0xe0, 0x81, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0x07, 0x8c, 0x8f, 0xf1, 0x30, 0xc0, 0x7f, 0xe0, 0x00, 0x38, 0xe0,
   0x00, 0x1d, 0x70, 0x04, 0x7f, 0x00, 0x08, 0x8f, 0xf1, 0x18, 0x00, 0xfc,
   0xf0, 0x00, 0x38, 0xfc, 0x87, 0xff, 0xf0, 0x07, 0x3e, 0x00, 0x18, 0x9e,
   0x71, 0x18, 0x00, 0x7c, 0x70, 0x00, 0x1c, 0x0c, 0x87, 0xe0, 0xf0, 0x07,
   0x3f, 0x40, 0x3e, 0x9c, 0x79, 0x7c, 0x02, 0xfc, 0x70, 0x00, 0x1c, 0x0e,
   0x0e, 0xe0, 0x38, 0x00, 0x3e, 0x78, 0x77, 0xbc, 0x3b, 0x3e, 0x1f, 0x7c,
   0x78, 0x00, 0x1c, 0xff, 0x07, 0xfa, 0x38, 0x00, 0x3e, 0x7e, 0xe6, 0xf8,
   0x3f, 0x0f, 0x7f, 0xfc, 0x78, 0x00, 0x1c, 0xff, 0xc7, 0xff, 0x38, 0x00,
   0x3e, 0xff, 0xc1, 0x0f, 0x70, 0x87, 0xff, 0x78, 0x70, 0x00, 0x1c, 0x07,
   0xe0, 0x71, 0x38, 0x00, 0xbf, 0xcf, 0xc7, 0x03, 0xc0, 0xe3, 0xf3, 0xfd,
   0xf0, 0x60, 0x0e, 0x07, 0xe0, 0x70, 0x1c, 0x00, 0xfe, 0x07, 0xde, 0x00,
   0x00, 0xf3, 0xe1, 0x7f, 0xf0, 0x3f, 0x0e, 0x0f, 0xf2, 0x70, 0x1c, 0x00,
   0xff, 0x07, 0x7c, 0x00, 0x00, 0xff, 0xc0, 0xff, 0xe0, 0x3f, 0x0e, 0xfe,
   0xe3, 0x7f, 0x1c, 0x00, 0xfe, 0x03, 0x39, 0x00, 0x00, 0xd0, 0xe7, 0x7f,
   0x80, 0x07, 0x0e, 0x78, 0xc1, 0x3b, 0x1c, 0x00, 0xfe, 0x7f, 0x1f, 0x00,
   0x00, 0x00, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x7c,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x0f, 0x00,
   0x00, 0x70, 0xe0, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0xc7, 0x0f, 0x00, 0x00, 0xf0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0f, 0xe8, 0x2f, 0xf0, 0xff, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfd, 0xef, 0xff,
   0xff, 0xff, 0xbf, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0x00, 0x00, 0x70, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfe, 0xf1, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x7f,
   0x00, 0x2e, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0xff, 0xc0, 0xff, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0x80, 0xff, 0xff, 0xff, 0xff, 0x01, 0x7f, 0xe0, 0xff, 0x38, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0xff,
   0xf0, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xa0, 0xc1, 0xff,
   0xff, 0x03, 0x05, 0x7c, 0xf0, 0x00, 0x38, 0x9c, 0x83, 0x03, 0x00, 0x00,
   0x1f, 0x20, 0xf2, 0xff, 0xff, 0x6f, 0x04, 0xfc, 0xf0, 0x00, 0x38, 0x9e,
   0x83, 0x03, 0x00, 0x00, 0x3e, 0x20, 0xf0, 0xff, 0xff, 0x07, 0x04, 0x7c,
   0xe0, 0x07, 0x3c, 0x87, 0xc3, 0x01, 0x00, 0x00, 0xfe, 0x20, 0xc0, 0xff,
   0xff, 0x03, 0x04, 0xff, 0xc0, 0x3f, 0x9c, 0x83, 0xc3, 0x01, 0x00, 0x00,
   0xfe, 0x01, 0x00, 0xff, 0xff, 0x01, 0x80, 0x7f, 0x00, 0x7f, 0xfc, 0x01,
   0xe7, 0x00, 0x00, 0x00, 0xff, 0x17, 0x00, 0x80, 0x00, 0x00, 0xe4, 0xff,
   0x00, 0x78, 0xfc, 0x01, 0x77, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x7f, 0x00, 0x70, 0xdc, 0x03, 0x77, 0x00, 0x00, 0x00,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x18, 0x78, 0x9e, 0x03,
   0x3f, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f,
   0xfc, 0x3f, 0x0e, 0x07, 0x1f, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0xff, 0xfc, 0x3f, 0x0e, 0x07, 0x1f, 0x00, 0x00, 0x00,
   0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f, 0xe0, 0x03, 0x0e, 0x0f,
   0x0e, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0x0f,
   0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x03, 0x00,
   0x00, 0xc0, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
   0x00, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x03, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x00, 0x00, 0xfe, 0x3f, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xbf,
   0xfa, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00 };

//pio device monitor -p COM5 -b 115200
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
#include <U8g2lib.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>

#include <EEPROM.h>

#endif
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 

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
uint8_t NODEID    =    1 ;   // keep UNIQUE for each node on same network
#define NETWORKID     100  // keep IDENTICAL on all nodes that talk to each other
uint8_t GATEWAYID  =   2 ;   // "central" node | Target ID

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
#define Button_Blue 25
#define Button_Yellow 26
#define Button_Green 27
#define Button_Red 14


#define buzzerPin 32
#define LEDpin 33

int TRANSMITPERIOD = 1000; //transmit a packet to gateway so often (in ms)
//char payload[] = "Turn on WiFi!";
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
void BuzzerHandler();
void LEDHandler();
void ConnectToWifi(uint16_t timeout);

uint16_t SENDERID = 0;
uint16_t TARGETID=  0;


bool Buzzer_ON = true;
bool LED_ON = true;
bool Display_ON = true;
bool InfoPanelSwitch = true;
bool LED_Settings = false;
bool OverwriteBuzzer = false;
bool OverwriteLED = false;

bool WifiStatus = false;

const char *string_list = 
  "RF settings\n"
  "ID settings\n"
  "Buzzer/LED ON/OFF\n"
  "Signal Decoding\n"
  "Buzzer/LED Test\n"
  "ClearSky Logo\n"
  "Wifi Settings\n"
  "Reset Settings";

uint8_t Display_selection = 0;
uint8_t Display_SettingsSelection = 0;
bool Display_SubSettingsSelection = 0;

bool MenuFlag = false;
unsigned long MenuTimer = 0;
bool DisplayMenu = false;

uint8_t LEDIntensity=255;
unsigned long previousMillis_LEDTimer=0;
unsigned long previousMillis_LEDOffTimer=0;
unsigned long previousMillis_LEDBuiltInOffTimer=0;
int16_t LEDTimer=500;
unsigned long MSGRecivedTime=0;

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




bool Button_Blue_state = 1;
bool Button_Yellow_state = 1;
bool Button_Green_state = 1;
bool Button_Red_state = 1;
bool Button_Blue_state_Prev = 1;
bool Button_Yellow_state_Prev = 1;
bool Button_Green_state_Prev = 1;
bool Button_Red_state_Prev = 1;

bool SendMsgFlag = false;
bool ReciveMsgFlag = true;
bool SignalDecoding = true;


const uint8_t GraphResPixel = 5;
const uint8_t RSSI_LogSize = 25;//126/GraphResPixel;//20
int RSSI_Log[RSSI_LogSize];
uint8_t First_X;
uint8_t Second_X;
int8_t First_RSSI ;
int8_t Second_RSSI;
int8_t First_Y;
int8_t Second_Y;
uint8_t VisualiseSection;


int8_t BuzzerIntensity= 0;
uint8_t BuzzerVolume = 255;
unsigned long previousMillis_BuzzerTimer=0;
unsigned long previousMillis_BuzzerOffTimer=0;
int16_t BuzzerTimer=500;

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
    for(int i = 0; i < RSSI_LogSize;  i++ )//clear IncomingMsg[] array
      RSSI_Log[i] = -100;

      pinMode(Button_Blue, INPUT_PULLUP);
      pinMode(Button_Yellow, INPUT_PULLUP);
      pinMode(Button_Green, INPUT_PULLUP);
      pinMode(Button_Red, INPUT_PULLUP);

      pinMode(LEDpin, OUTPUT);
      pinMode(buzzerPin, OUTPUT);
      pinMode(LED_BUILTIN, OUTPUT);

      analogWrite(LEDpin,0);



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
//#define Button_Blue 25
//#define Button_Yellow 26
//#define Button_Green 27
//#define Button_Red 14
u8g2.begin(27,26,25,U8X8_PIN_NONE,U8X8_PIN_NONE,14);  
//u8g2.begin();
//bool U8G2::begin(uint8_t menu_select_pin, uint8_t menu_next_pin, uint8_t menu_prev_pin, uint8_t menu_up_pin = U8X8_PIN_NONE, uint8_t menu_down_pin = U8X8_PIN_NONE, uint8_t menu_home_pin = U8X8_PIN_NONE)
u8g2.enableUTF8Print();
//u8g2.setDisplayRotation(U8G2_R2);

u8g2.clearBuffer();
  u8g2.drawXBM( 0, 0, ClearSky_Startup_width, ClearSky_Startup_height, ClearSky_Startup_bits);
  u8g2.sendBuffer();

  digitalWrite(buzzerPin, 1);delay(50);
  digitalWrite(buzzerPin, 0);delay(50);
  digitalWrite(buzzerPin, 1);delay(50);
  digitalWrite(buzzerPin, 0);delay(50);
  digitalWrite(buzzerPin, 0);delay(1000);

esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
esp_task_wdt_add(NULL); //add current thread to WDT watch

ConnectToWifi(1000);

//WiFi.mode(WIFI_STA);
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
  WiFi.mode(WIFI_OFF);WifiStatus = false;






EEPROM.begin(255);  //Initialize EEPROM
  //ReadEEprom Values:
ReciveMsgFlag   = EEPROM.read(0); Serial.printf("ReciveMsgFlag   : %d\n", ReciveMsgFlag   );
SendMsgFlag     = EEPROM.read(1); Serial.printf("SendMsgFlag     : %d\n", SendMsgFlag     );
BITRATE_Counter = EEPROM.read(2); Serial.printf("BITRATE_Counter : %d\n", BITRATE_Counter );
PowerLevel      = EEPROM.read(3); Serial.printf("PowerLevel      : %d\n", PowerLevel      );
NODEID          = EEPROM.read(4); Serial.printf("NODEID          : %d\n", NODEID          );
GATEWAYID       = EEPROM.read(5); Serial.printf("GATEWAYID       : %d\n", GATEWAYID       );
spy             = EEPROM.read(6); Serial.printf("spy             : %d\n", spy             );
Buzzer_ON       = EEPROM.read(7); Serial.printf("Buzzer_ON       : %d\n", Buzzer_ON       );
LED_ON          = EEPROM.read(8); Serial.printf("LED_ON          : %d\n", LED_ON          );
LEDIntensity    = EEPROM.read(9); Serial.printf("LEDIntensity    : %d\n", LEDIntensity    );
SignalDecoding  = EEPROM.read(10);Serial.printf("SignalDecoding  : %d\n", SignalDecoding  );

radio.setAddress(NODEID);
radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
radio.setPowerLevel(PowerLevel);
radio.spyMode(spy);

//In case of first Flash, when the EEPROM untouched, set the default values.
//(*When the EEPROM untouched, than every value is 255)
if(EEPROM.read(0)== 255){ReciveMsgFlag   = 1;  EEPROM.write(0, ReciveMsgFlag);}
if(EEPROM.read(1)== 255){SendMsgFlag     = 0;  EEPROM.write(1, SendMsgFlag);}
if(EEPROM.read(2)== 255){BITRATE_Counter = 6;  EEPROM.write(2, BITRATE_Counter);}
if(EEPROM.read(3)== 255){PowerLevel      = 23; EEPROM.write(3, PowerLevel);}
if(EEPROM.read(4)== 255){NODEID          = 2;  EEPROM.write(4, NODEID);}
if(EEPROM.read(5)== 255){GATEWAYID       = 1;  EEPROM.write(5, GATEWAYID);}
if(EEPROM.read(6)== 255){spy             = 1;  EEPROM.write(6, spy);}
if(EEPROM.read(7)== 255){Buzzer_ON       = 1;  EEPROM.write(7, Buzzer_ON);}
if(EEPROM.read(8)== 255){LED_ON          = 1;  EEPROM.write(8, LED_ON);}
if(EEPROM.read(9)== 255){LEDIntensity    = 255;EEPROM.write(9, LEDIntensity);}
if(EEPROM.read(10)== 255){SignalDecoding  = 1;  EEPROM.write(10,SignalDecoding);}

if(
ReciveMsgFlag  == 255 ||
SendMsgFlag    == 255 ||
BITRATE_Counter== 255 ||
PowerLevel     == 255 ||
NODEID         == 255 ||
GATEWAYID      == 255 ||
spy            == 255 ||
Buzzer_ON      == 255 ||
LED_ON         == 255 ||
//LEDIntensity   == 255 ||
SignalDecoding == 255){
  EEPROM.commit();
}


}



long lastPeriod = 0;
void loop() {
  DisplayUpdater();
  ProcessSerialInput();
  if(ReciveMsgFlag == true){RFM_Recive_msg();}
  if(SendMsgFlag == true){RFM_Send_msg();}
  ButtonHandler();
  BuzzerHandler();
  LEDHandler();
  WatchDogFeeder();
  ArduinoOTA.handle();
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
//h   BuzzerVolume++
//j   BuzzerVolume--
//g   Delay 5 sec (to test WDT)
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    if (input >= 48 && input <= 57) //[0,9]
    {
      TRANSMITPERIOD = 100 * (input-48);
      if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 5000;
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
      if (BITRATE_Counter>21) {BITRATE_Counter = 21;}
      radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
      radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
      Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
    }
    if (input == 'p')
    {
      ResetRadio(); 
      radio.initialize(FREQUENCY,NODEID,NETWORKID);
    }
    if (input == 'k')
    {
      WiFi.mode(WIFI_STA);
      WifiStatus = true;
    }
    if (input == 'l')
    {
      Serial.printf("Connection status: %s\n", WifiStatusReturn[WiFi.status()]);
    }
    //BuzzerVolume
    if (input == 'h')
    {
      //WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      WifiStatus = false;
    }
    if (input == 'j')
    {
      ConnectToWifi(5000);
    }
    if (input == 'g')
    {
      delay(5000);
    }
    if (input == '-')
    {
      ESP.restart();
    }
    if (input == '.')//read eeprom
    {
      for(int i =0; i<255;i++){
        Serial.printf("EEPROM[%d]: %d\n", i, EEPROM.read(i));
      }
    }
    

  }

}

void RFM_Recive_msg(){
if (radio.receiveDone())
  {
    MSGRecivedTime = millis();

    Serial.print("#[");
    Serial.print(++packetCount);
    Serial.print(']');
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    if (spy) Serial.print("to [");Serial.print(radio.TARGETID, DEC);Serial.print("] ");
    SENDERID = radio.SENDERID;
    TARGETID = radio.TARGETID;

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
    Serial.println();


for(int i=RSSI_LogSize-1; i>0;i--){
  RSSI_Log[i]=RSSI_Log[i-1];
}
RSSI_Log[0] = CurrentRSSI;


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


    //
    //Blink(LED_BUILTIN,3);
    //Blink(LEDpin,1);
    
  }
    
    //if (millis() - previousMillis_LEDTimer >= LEDTimer) {
    //  previousMillis_LEDTimer = millis();
    //  digitalWrite(LED_BUILTIN,1);
    //}

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
      char payload[] = "Turn on WiFi!";
      sendSize = 13;

      Serial.print("Sending[");
      Serial.print(sendSize);
      Serial.print("]: ");
      for(byte i = 0; i < sendSize; i++){
        Serial.print((char)payload[i]);
        SentMsg[i] = (char)payload[i];
      }
        
      sentMSGCounter++;
      //sendSize++;
      //if(sendSize>5){
      //  sendSize=1;
      //}
      radio.send(GATEWAYID, payload, sendSize);

      //char payload[] = "ABCDEFGHIJK";
      //if (radio.sendWithRetry(GATEWAYID, payload, sendSize))
      // Serial.print(" ok!");
      //else Serial.print(" nothing...");
    }
    //sendSize = (sendSize + 1) % 31;
    //sendSize = (sendSize + 1) % 4;

    Serial.println();
    //Blink(LED_BUILTIN,1);
    //Blink(33,3);
    digitalWrite(LED_BUILTIN,1);
    previousMillis_LEDBuiltInOffTimer = millis();
  }
  if(millis() - previousMillis_LEDBuiltInOffTimer >= 10){
      digitalWrite(LED_BUILTIN,0);
    }
}

void DisplayUpdater(){

  if( Display_ON == false){
    u8g2.setPowerSave(1);
  }else{
    u8g2.setPowerSave(0);
    if (millis() - previousMillis_DisplayUpdater >= 100) {
      previousMillis_DisplayUpdater = millis();

      u8g2.clearBuffer(); // clear the internal memory
      AliveAnimation();
      //1 - "RF settings\n"
      //2 - "ID settings\n"
      //3 - "Buzzer/LED ON/Off\n"
      //4 - "Signal Decoding\n"
      //5 - "Buzzer/LED Test\n"
      //6 - "ClearSky Logo\n";
      //7 - "Wifi Settings\n"
      //8 - "Reset Settings\n";
      switch(Display_selection){
        case 0://Main Display

        u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
        u8g2.setCursor(0,8);
        u8g2.printf("ID:%d T:%d S:%s RSSI:%d",NODEID, GATEWAYID, spy?"ON":"OFF",CurrentRSSI);//u8g2.print(SentMsg);
        if (ReciveMsgFlag == true){
        u8g2.setCursor(0,16); 
        if(SignalDecoding==true){
          uint16_t asd = radio.SENDERID;
          uint16_t ads2 = radio.TARGETID;
          u8g2.printf("Got[%d][%d][%d]%s",packetCount, SENDERID, TARGETID, IncomingMsg_char);
        }else{
          u8g2.printf("Got[%d]",packetCount);
        }
        //print(radio.SENDERID, DEC)
        }
        if (SendMsgFlag == true){
        u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
        u8g2.setCursor(0,24); 
        u8g2.printf("Sent[%d]%s",sentMSGCounter,SentMsg);//u8g2.print(SentMsg); 
        sentMSGCounter_Prev = sentMSGCounter;
        }

        u8g2.setCursor(0,32);
        if(InfoPanelSwitch == true){
          u8g2.printf("Led:%s Buzzer:%s WiFi:%s",LED_ON?"ON":"OFF",Buzzer_ON?"ON":"OFF",WifiStatus?"ON":"OFF");
        }else if(InfoPanelSwitch == false){
          u8g2.printf("Power:%d BitRate:%d",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
        }
        
        


        u8g2.drawFrame(0,33,128,31);
        u8g2.setFont(u8g2_font_u8glib_4_tf);

        u8g2.setCursor(0,39); 
        u8g2.printf("-30dbi");
        u8g2.setCursor(0,62); 
        u8g2.printf("-100dbi");
        u8g2.drawLine(25, 33, 25, 63);



        VisualiseSection = 100/GraphResPixel; //-->20

        for (int i = 0;i<VisualiseSection; i++){
          First_X = 25+i*GraphResPixel;
          Second_X = 25+5+i*GraphResPixel;

          First_RSSI = RSSI_Log[VisualiseSection-i];
          Second_RSSI = RSSI_Log[VisualiseSection-1-i];

          First_Y = map(First_RSSI,-100,-30,62,34);
          Second_Y = map(Second_RSSI,-100,-30,62,34);

        //  Serial.printf("|i:%d F_X:%d F_Y:%d/%d S_X:%d S_Y:%d/%d| \n", i, First_X,First_RSSI,First_Y,Second_X,Second_RSSI,Second_Y);
          u8g2.drawLine(First_X, First_Y, Second_X, Second_Y);
        }

          break;

        case 1://RF settings
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "RF settings" );


          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 20, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 20, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,28);u8g2.printf("Recive RF:");
          u8g2.setCursor(63,28);u8g2.printf("%s",ReciveMsgFlag?"true":"false");
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 30, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 30, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,38);u8g2.printf("Send RF:");
          u8g2.setCursor(63,38);u8g2.printf("%s",SendMsgFlag?"true":"false");
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 40, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 40, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,48);u8g2.printf("BitRate:");
          u8g2.setCursor(63,48);u8g2.printf("%d",BITRATE_Meaning[BITRATE_Counter]);
          if(Display_SettingsSelection==3 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 50, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==3 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 50, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,58);u8g2.printf("PowerLvl:");
          u8g2.setCursor(63,58);u8g2.printf("%d",PowerLevel);
          

          
          if(Button_Blue_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection--;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Button_Yellow_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection++;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Display_SettingsSelection<0 || Display_SettingsSelection >= 4){Display_SettingsSelection=0;}

          if(Button_Green_state == 0){Display_SubSettingsSelection = !Display_SubSettingsSelection;}

          if(Display_SettingsSelection == 0 &&Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            ReciveMsgFlag = !ReciveMsgFlag;
            EEPROM.write(0, ReciveMsgFlag);
            Serial.printf("ReciveMsgFlag: %d\n",ReciveMsgFlag);
          }
          if(Display_SettingsSelection == 1 && Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            SendMsgFlag = !SendMsgFlag;
            EEPROM.write(1, SendMsgFlag);
            Serial.printf("SendMsgFlag: %d\n",SendMsgFlag);
          }

          if(Display_SettingsSelection == 2 && Display_SubSettingsSelection==1){
            if(Button_Blue_state==0){
              BITRATE_Counter++;
              if (BITRATE_Counter>21) {BITRATE_Counter = 21;}
              EEPROM.write(2, BITRATE_Counter);
              radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
              radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
              Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
            }else if(Button_Yellow_state==0){
              BITRATE_Counter--;
              if (BITRATE_Counter<0) {BITRATE_Counter = 0;}
              EEPROM.write(2, BITRATE_Counter);
              radio.writeReg(REG_BITRATEMSB, BITRATE[BITRATE_Counter][1]); // setup- function, after radio.initialize(...)
              radio.writeReg(REG_BITRATELSB, BITRATE[BITRATE_Counter][0]);   // setup- function, after radio.initialize(...)
              Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
            }
          }
          if(Display_SettingsSelection == 3 && Display_SubSettingsSelection==1){
            if(Button_Blue_state==0){
              PowerLevel++;
              if (PowerLevel>23) {PowerLevel = 23;}
              EEPROM.write(3, PowerLevel);
              radio.setPowerLevel(PowerLevel);
              Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
            }else if(Button_Yellow_state==0){
              PowerLevel--;
              if (PowerLevel<0) {PowerLevel = 0;}
              EEPROM.write(3, PowerLevel);
              radio.setPowerLevel(PowerLevel);
              Serial.printf("PL: %d BR: %d\n",radio.getPowerLevel(), BITRATE_Meaning[BITRATE_Counter]);
            }          
          }
          
          if(Button_Red_state==0){Display_selection=0;EEPROM.commit();}

          break;      

        case 2://RF ID settings 
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "RF ID settings" );

          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 20, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 20, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,28);u8g2.printf("Node ID:");
          u8g2.setCursor(63,28);u8g2.printf("%d",NODEID);
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 30, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 30, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,38);u8g2.printf("Target ID:");
          u8g2.setCursor(63,38);u8g2.printf("%d",GATEWAYID);
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 40, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 40, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,48);u8g2.printf("Spy mode:");
          u8g2.setCursor(63,48);u8g2.printf("%s",spy?"true":"false");

          if(Button_Blue_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection--;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Button_Yellow_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection++;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Display_SettingsSelection<0 || Display_SettingsSelection >= 3){Display_SettingsSelection=0;}

          if(Button_Green_state == 0){Display_SubSettingsSelection = !Display_SubSettingsSelection;}

          

          if(Display_SettingsSelection == 0 && Display_SubSettingsSelection==1){
            if(Button_Blue_state==0){
              NODEID++;
              EEPROM.write(4, NODEID);
              radio.setAddress(NODEID);
            }else if(Button_Yellow_state==0){
              NODEID--;
              EEPROM.write(4, NODEID);
              radio.setAddress(NODEID);
            }
          }

          if(Display_SettingsSelection == 1 && Display_SubSettingsSelection==1){
            if(Button_Blue_state==0){
              GATEWAYID++;
              EEPROM.write(5, GATEWAYID);
            }else if(Button_Yellow_state==0){
              GATEWAYID--;
              EEPROM.write(5, GATEWAYID);
            }
          }

          if(Display_SettingsSelection == 2 &&Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            spy = !spy;
            EEPROM.write(6, spy);
            radio.spyMode(spy);
            Serial.printf("spy: %d\n",spy);
          }



          if(Button_Red_state==0){Display_selection=0;EEPROM.commit();}
          break;
           
        case 3://Buzzer/Led On/Off
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Buzzer/LED ON/Off" );


          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 20, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 20, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,28);u8g2.printf("Buzzer_ON:");
          u8g2.setCursor(63,28);u8g2.printf("%s",Buzzer_ON?"true":"false");
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 30, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 30, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,38);u8g2.printf("Led_ON:");
          u8g2.setCursor(63,38);u8g2.printf("%s",LED_ON?"true":"false");
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 40, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 1){u8g2.drawBox(63, 40, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,48);u8g2.printf("Led Value:");
          u8g2.setCursor(63,48);u8g2.printf("%d",LEDIntensity);
          
          
          if(Button_Green_state == 0){Display_SubSettingsSelection = !Display_SubSettingsSelection;}
          if(Button_Blue_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection--;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Button_Yellow_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection++;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Display_SettingsSelection<0 || Display_SettingsSelection >= 3){Display_SettingsSelection=0;}

          if(Display_SettingsSelection == 0 && Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            Buzzer_ON = !Buzzer_ON;
            EEPROM.write(7, Buzzer_ON);
            Serial.printf("Buzzer_ON: %d\n",Buzzer_ON);
          }
          if(Display_SettingsSelection == 1 && Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            LED_ON = !LED_ON;
            EEPROM.write(8, LED_ON);
            Serial.printf("LED_ON: %d\n",LED_ON);
          }
          if(Display_SettingsSelection == 2 && Display_SubSettingsSelection==1){
            LED_Settings = true;
            analogWrite(LEDpin,LEDIntensity);
            if(Button_Blue_state==0){
              LEDIntensity=LEDIntensity+2;
              EEPROM.write(9, LEDIntensity);
            }else if(Button_Yellow_state == 0){
              LEDIntensity=LEDIntensity-2;
              EEPROM.write(9, LEDIntensity);
            }
            Serial.printf("LEDIntensity: %d\n",LEDIntensity);
          }
          if(Display_SettingsSelection != 2){LED_Settings=false;}
          if(Button_Red_state==0){Display_selection=0;LED_Settings=false;EEPROM.commit();}
          break;

        case 4://Signal Decoding //SignalDecoding
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Signal Decoding" );


          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 20, 100, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 1){u8g2.drawBox(100, 20, 63, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,28);u8g2.printf("Signal Decoding:");
          u8g2.setCursor(100,28);u8g2.printf("%s",SignalDecoding?"true":"false");
          
          if(Button_Green_state == 0){Display_SubSettingsSelection = !Display_SubSettingsSelection;}

          if(Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            SignalDecoding = !SignalDecoding;
            EEPROM.write(10,SignalDecoding);
            Serial.printf("SignalDecoding: %d\n",SignalDecoding);
          }
          if(Button_Red_state==0){Display_selection=0;EEPROM.commit();}
          break;
        case 5://Buzzer/LED Test
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Buzzer/LED Test" );


          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 20, 100, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==0 && Display_SubSettingsSelection == 1){u8g2.drawBox(100, 20, 100, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,28);u8g2.printf("Overwrite Buzzer:");
          u8g2.setCursor(100,28);u8g2.printf("%s",OverwriteBuzzer?"true":"false");
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 30, 100, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==1 && Display_SubSettingsSelection == 1){u8g2.drawBox(100, 30, 100, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,38);u8g2.printf("Buzzer Value:");
          u8g2.setCursor(100,38);u8g2.printf("%d",BuzzerTimer);
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 40, 100, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==2 && Display_SubSettingsSelection == 1){u8g2.drawBox(100, 40, 100, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,48);u8g2.printf("Overwrite Led:");
          u8g2.setCursor(100,48);u8g2.printf("%s",OverwriteLED?"true":"false");
          if(Display_SettingsSelection==3 && Display_SubSettingsSelection == 0){u8g2.drawBox(0, 50, 100, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==3 && Display_SubSettingsSelection == 1){u8g2.drawBox(100, 50, 100, 8);u8g2.setDrawColor(2);}
          u8g2.setCursor(0,58);u8g2.printf("Led Value:");
          u8g2.setCursor(100,58);u8g2.printf("%d",LEDTimer);

          if(Button_Blue_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection--;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Button_Yellow_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection++;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Display_SettingsSelection<0 || Display_SettingsSelection >= 4){Display_SettingsSelection=0;}

          if(Button_Green_state == 0){Display_SubSettingsSelection = !Display_SubSettingsSelection;}

          if(Display_SettingsSelection == 0 &&Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            OverwriteBuzzer = !OverwriteBuzzer;
            Serial.printf("OverwriteBuzzer: %d\n",OverwriteBuzzer);
          }

          if(Display_SettingsSelection == 1 && Display_SubSettingsSelection==1){
            Buzzer_ON = true;
            if(Button_Blue_state==0){
              BuzzerTimer=BuzzerTimer+5;
            }else if(Button_Yellow_state==0){
              BuzzerTimer=BuzzerTimer-5;
            }
          }

          if(Display_SettingsSelection == 2 && Display_SubSettingsSelection==1 && (Button_Blue_state==0 || Button_Yellow_state==0)){
            OverwriteLED = !OverwriteLED;
            Serial.printf("OverwriteLED: %d\n",OverwriteLED);
          }

          if(Display_SettingsSelection == 3 && Display_SubSettingsSelection==1){
            LED_ON = true;
            if(Button_Blue_state==0){
              LEDTimer=LEDTimer+5;
            }else if(Button_Yellow_state==0){
              LEDTimer=LEDTimer-5;
            }
          }
          if(Button_Red_state==0){Display_selection=0;}

          break;

        case 6://ClearSky Logo
          u8g2.drawXBM( 0, 0, ClearSky_Startup_width, ClearSky_Startup_height, ClearSky_Startup_bits);
          if(Button_Red_state==0){Display_selection=0;}
          break;    

        case 7://Wifi Settings
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Wifi Settings" );

          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          u8g2.setCursor(0,22);u8g2.printf("SSID: Xiaomi11T");
          u8g2.setCursor(0,32);u8g2.printf("Pass: 11112222");
          u8g2.setCursor(0,42);u8g2.printf("WifiStatus: %s", WifiStatus?WifiStatusReturn[WiFi.status()]:"WiFi Off");
          u8g2.setCursor(0,52);u8g2.printf("IP: ");u8g2.print(WiFi.localIP());
          

          if(Display_SettingsSelection==1){u8g2.drawBox(0, 54, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==2){u8g2.drawBox(63, 54, 64, 8);u8g2.setDrawColor(2);}

          u8g2.drawButtonUTF8(63-63/2, 62, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Connect" );
          u8g2.drawButtonUTF8(63+63/2, 62, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Turn Off" );

          if(Button_Blue_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection--;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Button_Yellow_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection++;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Display_SettingsSelection<0 || Display_SettingsSelection >= 3){Display_SettingsSelection=0;}

          if(Display_SettingsSelection==2 && Button_Green_state==0){
            //Display_selection=0;
            WiFi.mode(WIFI_OFF);WifiStatus = false;
            Serial.printf("Wifi turning off!%d\n");
          }
          if(Display_SettingsSelection==1 && Button_Green_state==0 ){
            Serial.printf("Connect to Wifi! %d\n");
            ConnectToWifi(5000);
            //Display_selection=0;
            //Factory Reset Code:
           
          }


          if(Button_Red_state==0){Display_selection=0;}
          break;


        case 8://Reset Settings
          u8g2.setFont(u8g2_font_helvR08_tr);
          u8g2.drawButtonUTF8(63, 10, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Reset Factory Settings" );

          u8g2.setFontMode(1);  /* activate transparent font mode */
          u8g2.setDrawColor(1); /* color 1 for the box */
          u8g2.setCursor(0,28);u8g2.printf("Reseting EEPROM Values.");
          u8g2.setCursor(0,38);u8g2.printf("Are you sure?");

          if(Display_SettingsSelection==1){u8g2.drawBox(0, 50, 63, 8);u8g2.setDrawColor(2);}
          if(Display_SettingsSelection==2){u8g2.drawBox(63, 50, 64, 8);u8g2.setDrawColor(2);}

          u8g2.drawButtonUTF8(63-63/2, 58, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "OK" );
          u8g2.drawButtonUTF8(63+63/2, 58, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, "Cancel" );


    
          
          if(Button_Blue_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection--;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Button_Yellow_state==0 && Display_SubSettingsSelection == 0){Display_SettingsSelection++;Serial.printf("Display_SettingsSelection: %d\n",Display_SettingsSelection);}
          if(Display_SettingsSelection<0 || Display_SettingsSelection >= 3){Display_SettingsSelection=0;}

          if(Display_SettingsSelection==2 && Button_Green_state==0){
            Display_selection=0;
            Serial.printf("Cancel factory Reset. Nothing happend!%d\n");
          }
          if(Display_SettingsSelection==1 && Button_Green_state==0 ){
            Serial.printf("Factory Reset! %d\n");
            Display_selection=0;
            //Factory Reset Code:
            ReciveMsgFlag   = 1;    EEPROM.write(0, ReciveMsgFlag);
            SendMsgFlag     = 0;    EEPROM.write(1, SendMsgFlag);
            BITRATE_Counter = 6;    EEPROM.write(2, BITRATE_Counter);
            PowerLevel      = 23;   EEPROM.write(3, PowerLevel);
            NODEID          = 2;    EEPROM.write(4, NODEID);
            GATEWAYID       = 1;    EEPROM.write(5, GATEWAYID);
            spy             = 1;    EEPROM.write(6, spy);
            Buzzer_ON       = 1;    EEPROM.write(7, Buzzer_ON);
            LED_ON          = 1;    EEPROM.write(8, LED_ON);
            LEDIntensity    = 255;  EEPROM.write(9, LEDIntensity);
            SignalDecoding  = 1;    EEPROM.write(10,SignalDecoding);
            EEPROM.commit();
          }

          if(Button_Red_state==0){Display_selection=0;}
          break;      

      }

        u8g2.sendBuffer();          // transfer internal memory to the display
    

    }
  }


}




void AliveAnimation(){


  if (millis() - previousMillis_LoadingIcon >= 100) {
    // save the last time you blinked the LED
    previousMillis_LoadingIcon = millis();
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

void ButtonHandler(){

  Button_Blue_state = digitalRead(Button_Blue);
  Button_Yellow_state= digitalRead(Button_Yellow);
  Button_Green_state = digitalRead(Button_Green);
  Button_Red_state= digitalRead(Button_Red);




  if(Button_Blue_state_Prev!=Button_Blue_state){
    Serial.printf("Button-B: %d\n", Button_Blue_state);
    if(Button_Blue_state==0 && Display_selection ==0){
      LED_ON = !LED_ON;
      Serial.printf("LED_ON: %d\n", LED_ON);

    }
  }
  if(Button_Yellow_state_Prev!=Button_Yellow_state){
    Serial.printf("Button-Y: %d\n", Button_Yellow_state);
    if(Button_Yellow_state==0 && Display_selection ==0){
      Buzzer_ON = !Buzzer_ON;
      Serial.printf("Buzzer_ON: %d\n", Buzzer_ON);


    }
  }
  if(Button_Green_state_Prev!=Button_Green_state){
    Serial.printf("Button-G: %d\n", Button_Green_state);
    if(Button_Green_state==0 && Display_selection ==0){
      Display_ON = !Display_ON;
      Serial.printf("Display_ON: %d\n", Display_ON);

    }
  }
  if(Button_Red_state_Prev!=Button_Red_state){
    Serial.printf("Button_R: %d\n", Button_Red_state);
    if(Button_Red_state == 0 && Display_selection == 0){
      InfoPanelSwitch = !InfoPanelSwitch;

    }
  }


  if(//start reseting to default values
  Button_Blue_state == 0 &&
  Button_Yellow_state == 0 &&
  MenuFlag == false
){
  MenuTimer = millis();
  MenuFlag = true;
  Serial.printf("Menu Timer Started!\n");
}

if(// iff button relesed cancel reseting
  (Button_Blue_state == 1 ||
  Button_Yellow_state == 1) &&
  MenuFlag == true
){
  MenuFlag = false;
  Serial.printf("Cancel reseting!\n");
  }

if(millis()-MenuTimer>3000 &&
  Button_Blue_state == 0 &&
  Button_Yellow_state == 0 &&
  MenuFlag == true
){
  DisplayMenu = 1;
  Serial.printf("DisplayMenu now!\n");
  MenuFlag = false;
  Display_SettingsSelection = 0;
  Display_SubSettingsSelection = 0;

u8g2.setFont(u8g2_font_6x12_tr);

esp_task_wdt_init(6000, true); //enable panic so ESP32 restarts
Display_selection = u8g2.userInterfaceSelectionList(
    "Echo Detector Menu",
    Display_selection, 
    string_list);
Serial.printf("current_selection: %d\n",Display_selection);
esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts

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

void BuzzerHandler(){
  if(Buzzer_ON == true){
    //do buzzing
    if(OverwriteBuzzer==false){
      BuzzerTimer = map(CurrentRSSI,-100,-30,500,0);
    }
    if(BuzzerTimer>=500){BuzzerTimer = 500;}
    if(BuzzerTimer<=0){BuzzerTimer = 0;}


    if(BuzzerTimer != 0 && millis() - previousMillis_BuzzerOffTimer >= 1){
    previousMillis_BuzzerOffTimer = millis();
    digitalWrite(buzzerPin,0);
    }
    if (millis() - previousMillis_BuzzerTimer >= BuzzerTimer && millis() - MSGRecivedTime <= 500) {
      previousMillis_BuzzerTimer = millis();
      digitalWrite(buzzerPin,1);
    }
  }else if(Buzzer_ON == false){
    digitalWrite(buzzerPin,0);
  }
}

void LEDHandler()
{
  if(LED_ON == true){
    //Do LED
    if(OverwriteLED==false){
      LEDTimer = map(CurrentRSSI,-120,-30,500,0);
    }
    if(LEDTimer>=500){LEDTimer = 500;}
    if(LEDTimer<=0){LEDTimer = 0;}
    //Serial.printf("LEDTimer: %d\n",LEDTimer);
    if(LEDTimer != 0 && millis() - previousMillis_LEDOffTimer >= 10){
      previousMillis_LEDOffTimer = millis();
      analogWrite(LEDpin,0);
    }
    if (millis() - previousMillis_LEDTimer >= LEDTimer && millis() - MSGRecivedTime <= 500) {
      previousMillis_LEDTimer = millis();
      analogWrite(LEDpin,LEDIntensity);
    }
  }else if(LED_ON == false && LED_Settings==false){
    analogWrite(LEDpin,0);
  }
}


void ConnectToWifi(uint16_t timeout){
    WiFi.mode(WIFI_STA);
    WifiStatus = true;
    WiFi.begin(ssid, password);
    uint8_t WifiTimeoutCounter = 0;
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis()-start < timeout){}

    if(millis()-start <= timeout){
      Serial.printf("Successfully connected to the Wifi! Connection time: %dms\n", millis()-start);
      //Serial.printf("Local IP address: %s\n", WiFi.localIP());
      Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
      
    }else{
      Serial.println("Timeout! Couldn't connect!"); 
    }
}