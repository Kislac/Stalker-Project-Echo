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
#endif
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  
//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE ************
//*********************************************************************************************
// Address IDs are 10bit, meaning usable ID range is 1..1023
// Address 0 is special (broadcast), messages to address 0 are received by all *listening* nodes (ie. active RX mode)
// Gateway ID should be kept at ID=1 for simplicity, although this is not a hard constraint
//*********************************************************************************************
#define NODEID        2    // keep UNIQUE for each node on same network
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



//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  


int TRANSMITPERIOD = 500; //transmit a packet to gateway so often (in ms)
char payload[] = "123-ABCDEFGHIJ";
char buff[20];
byte sendSize=0;
boolean requestACK = false;
//SPIFlash flash(SS_FLASHMEM, 0xEF30); //EF30 for 4mbit  Windbond chip (W25X40CL)
char SentMsg[64];
char IncomingMsg[64];

int previousMillis_StandBy;
unsigned long currentMillis = millis();
unsigned long previousMillis_LoadingIcon = 0;
unsigned long previousMillis_DisplayUpdater = 0;
int IconState = 0;
int CurrentRSSI = 0;
uint32_t packetCount = 0;
char IncomingMsg_char[230];
int IncomingMsg_int[230];
int IncomingMSG_Lenght;
bool RFM_Recive_msg_DispFlag = false;
bool RFM_Send_msg_DispFlag = false;
int sentMSGCounter = 0;
int packetCount_Prev;
int sentMSGCounter_Prev;


#ifdef ENABLE_ATC
  RFM69_ATC radio(5,16,true);
#else
  RFM69 radio(5,16,true);
#endif

void ResetRadio()
{
  digitalWrite(RESET_PIN, HIGH);
  delay(50);
  digitalWrite(RESET_PIN, LOW);
  Serial.println("Radio reseted!");
}
void AliveAnimation();
void DisplayUpdater();
void ProcessSerialInput();
void RFM_Recive_msg();
void RFM_Send_msg();

void setup() {
  Serial.begin(SERIAL_BAUD);
  ResetRadio();
  radio.initialize(FREQUENCY,NODEID,NETWORKID);

radio.writeReg(REG_BITRATEMSB, RF_BITRATEMSB_25000); // setup- function, after radio.initialize(...)
radio.writeReg(REG_BITRATELSB, RF_BITRATELSB_25000);   // setup- function, after radio.initialize(...)


#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif

#ifdef ENCRYPTKEY
  radio.encrypt(ENCRYPTKEY);
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

  u8g2.begin();  
  u8g2.enableUTF8Print();
  //u8g2.setDisplayRotation(U8G2_R2);

#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)\n");
#endif
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

long lastPeriod = 0;
void loop() {

  currentMillis = millis(); 
  DisplayUpdater();

  ProcessSerialInput();
  RFM_Recive_msg();
  RFM_Send_msg();



  

  //check for any received packets
 

  
}

void RFM_Recive_msg(){
  RFM_Recive_msg_DispFlag = true;

 if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);

    CurrentRSSI = radio.RSSI;      
    Serial.print("   [RX_RSSI:");Serial.print(CurrentRSSI);Serial.print("]");


    packetCount++;
    for(int b = 0; b < sizeof(IncomingMsg);  ++b )//clear IncomingMsg[] array
        IncomingMsg[b] = (char)0;
    //memset(IncomingMsg, 0, sizeof(IncomingMsg)); 
      for (byte i = 0; i < radio.DATALEN; i++){
        Serial.print((char)radio.DATA[i]);
        IncomingMsg[i] = (char)radio.DATA[i];
      }


    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    Blink(LED_BUILTIN,3);
    Serial.println();
  }


}


void RFM_Send_msg(){
  RFM_Send_msg_DispFlag = true;
int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    lastPeriod=currPeriod;

    for(int b = 0; b < sizeof(SentMsg);  ++b )//clear SentMsg[]array
        SentMsg[b] = (char)0;


    //send FLASH id
    if(sendSize==0)
    {
      //sprintf(buff, "FLASH_MEM_ID:0x%X", flash.readDeviceId());
      byte buffLen=strlen(buff);
      sentMSGCounter++;
      if (radio.sendWithRetry(GATEWAYID, buff, buffLen))
        Serial.print(" ok!");
      else Serial.print(" nothing...");
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
      sentMSGCounter++;
      if (radio.sendWithRetry(GATEWAYID, payload, sendSize))
       Serial.print(" ok!");
      else Serial.print(" nothing...");
    }
    


    sendSize = (sendSize + 1) % 10;//31;
    Serial.println();
    Blink(LED_BUILTIN,1);
  }
}



void ProcessSerialInput(){
// process any serial input
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    if (input == 'r') //d=dump all register values
      radio.readAllRegs();
    if (input == 'E') //E=enable encryption
      radio.encrypt(ENCRYPTKEY);
    if (input == 'e') //e=disable encryption
      radio.encrypt(null);
    //if (input == 'p')
    //{
    //  promiscuousMode = !promiscuousMode;
    //  radio.promiscuous(promiscuousMode);
    //  Serial.print("Promiscuous mode ");Serial.println(promiscuousMode ? "on" : "off");
    //}      
    if (input >= 48 && input <= 57) //[0,9]
    {
      TRANSMITPERIOD = 100 * (input-48);
      if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 1000;
      Serial.print("\nChanging delay to ");
      Serial.print(TRANSMITPERIOD);
      Serial.println("ms\n");
    }
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
    if (input == 'r') //d=dump register values
    {
      radio.readAllRegs();
    }
    if (input == 'E') //E=enable encryption
    {
     radio.encrypt(ENCRYPTKEY);
    }
    if (input == 'e') //e=disable encryption
    {
     radio.encrypt(null);
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


  if (currentMillis - previousMillis_LoadingIcon >= 1000) {
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
    //AliveAnimation();

  if (RFM_Recive_msg_DispFlag == true){
      u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
      u8g2.setCursor(0,7); 
      u8g2.print("RSSI: ");u8g2.print(CurrentRSSI);
      u8g2.setCursor(0,14); 
      u8g2.print("Got:[");u8g2.print(packetCount);u8g2.print(']');u8g2.print(IncomingMsg);
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