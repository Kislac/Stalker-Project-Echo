// **********************************************************************************
//            !!!!     ATTENTION:    !!!!
// This is just a simple receiving sketch that will work with most examples
// in the RFM69 library.
//
// If you're looking for the Gateway sketch to use with your RaspberryPi,
// as part of the PiGateway software interface (lowpowerlab.com/gateway),
// this is the wrong sketch. Use this sketch instead: PiGateway:
// https://github.com/LowPowerLab/RFM69/blob/master/Examples/PiGateway/PiGateway.ino
// **********************************************************************************
// Sample RFM69 receiver/gateway sketch, with ACK and optional encryption, and Automatic Transmission Control
// Passes through any wireless received messages to the serial port & responds to ACKs
// It also looks for an onboard FLASH chip, if present
// **********************************************************************************
// Copyright Felix Rusu 2016, http://www.LowPowerLab.com/contact
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
#endif
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  
//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define NODEID        1    //should always be 1 for a Gateway
#define NETWORKID     100  //the same on all nodes that talk to each other
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_433MHZ
//#define FREQUENCY     RF69_868MHZ
//#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//*********************************************************************************************
#define SERIAL_BAUD   115200
#define RESET_PIN 4

#ifdef ENABLE_ATC
  RFM69_ATC radio(5,16,true);
#else
  RFM69 radio;
#endif

//SPIFlash flash(SS_FLASHMEM, 0xEF30); //EF30 for 4mbit  Windbond chip (W25X40CL)
bool spy = false; //set to 'true' to sniff all packets on the same network

int previousMillis_StandBy;
unsigned long currentMillis = millis();
unsigned long previousMillis_LoadingIcon = 0;
unsigned long previousMillis_DisplayUpdater = 0;


int IconState = 0;
int CurrentRSSI = 0;
char IncomingMsg_char[230];
int IncomingMsg_int[230];
//int IncomingMSG_Lenght;
bool RFM_Recive_msg_DispFlag = false;
bool RFM_Send_msg_DispFlag = false;
int sentMSGCounter = 0;
int packetCount_Prev;
int sentMSGCounter_Prev;

char SentMsg[64];
char IncomingMsg[64];

void ResetRadio()
{
  digitalWrite(RESET_PIN, HIGH);
  delay(10);
  digitalWrite(RESET_PIN, LOW);
  Serial.println("Radio reseted!");
}


void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(10);
  ResetRadio();
  radio.initialize(FREQUENCY,NODEID,NETWORKID);

radio.writeReg(REG_BITRATEMSB, RF_BITRATEMSB_25000); // setup- function, after radio.initialize(...)
radio.writeReg(REG_BITRATELSB, RF_BITRATELSB_25000);   // setup- function, after radio.initialize(...)

#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
  radio.encrypt(ENCRYPTKEY);
  radio.spyMode(spy);
  //radio.setFrequency(916000000); //set frequency to some custom frequency
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);

u8g2.begin();  
  u8g2.enableUTF8Print();
  u8g2.setDisplayRotation(U8G2_R2);


#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)");
#endif
}

byte ackCount=0;
uint32_t packetCount = 0;

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

void loop() {
  //process any serial input

  currentMillis = millis(); 
  DisplayUpdater();

  ProcessSerialInput();
  RFM_Recive_msg();
  //RFM_Send_msg();

  

  
}

void RFM_Recive_msg(){
  RFM_Recive_msg_DispFlag = true;
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
    
    if (radio.ACKRequested())
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
      Serial.print(" - ACK sent.");

      // When a node requests an ACK, respond to the ACK
      // and also send a packet requesting an ACK (every 3rd one only)
      // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
      if (ackCount++%3==0)
      {
        Serial.print(" Pinging node ");
        Serial.print(theNodeID);
        Serial.print(" - ACK...");
        delay(3); //need this when sending right after reception .. ?
        if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 3))  // 0 = only 1 attempt, no retries
          Serial.print("ok!");
        else Serial.print("nothing");
      }
    }
    Serial.println();
    Blink(LED_BUILTIN,3);
  }
}


void ProcessSerialInput(){
if (Serial.available() > 0)
  {
    char input = Serial.read();
    if (input == 'r') //d=dump all register values
      radio.readAllRegs();
    if (input == 'E') //E=enable encryption
      radio.encrypt(ENCRYPTKEY);
    if (input == 'e') //e=disable encryption
      radio.encrypt(null);
    if (input == 'p')
    {
      spy = !spy;
      radio.spyMode(spy);
      Serial.print("SpyMode mode ");Serial.println(spy ? "on" : "off");
    }
    
    /*if (input == 'd') //d=dump flash area
    {
      Serial.println("Flash content:");
      int counter = 0;

      while(counter<=256){
        Serial.print(flash.readByte(counter++), HEX);
        Serial.print('.');
      }
      while(flash.busy());
      Serial.println();
    }
    if (input == 'D')
    {
      Serial.print("Deleting Flash chip ... ");
      flash.chipErase();
      while(flash.busy());
      Serial.println("DONE");
    }
    if (input == 'i')
    {
      Serial.print("DeviceID: ");
      word jedecid = flash.readDeviceId();
      Serial.println(jedecid, HEX);
    }*/
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
  }
}


void AliveAnimation(){


  if (currentMillis - previousMillis_LoadingIcon >= 250) {
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

  
  if(sentMSGCounter_Prev != sentMSGCounter || packetCount_Prev != packetCount){

  u8g2.clearBuffer(); // clear the internal memory
    AliveAnimation();

  if (RFM_Recive_msg_DispFlag == true){
      u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
      u8g2.setCursor(0,7); 
      u8g2.print("RSSI: ");u8g2.print(CurrentRSSI);
      u8g2.setCursor(0,14); 
      u8g2.print("Got:[");u8g2.print(packetCount);u8g2.print(']');u8g2.print(IncomingMsg_char);
      packetCount_Prev = packetCount; 
    }
  if (RFM_Send_msg_DispFlag == true){
      u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
      u8g2.setCursor(0,21); 
      u8g2.printf("Sent[%d]: %s",sentMSGCounter,SentMsg);//u8g2.print(SentMsg); 
      sentMSGCounter_Prev = sentMSGCounter;

    }


    u8g2.sendBuffer();          // transfer internal memory to the display
  }
}