//New receiving method. This method checks the Rx Fifo for any data it contains.
//It allows you to do several things in a loop.
//In addition, the gdo0 and gdo2 pin are not required.
//https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
//by Little_S@tan
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Arduino.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void DisplaySetup();
void DisplayUpdater();
void RadioSetup();
uint16_t combineFromBytes(uint8_t highByte, uint8_t lowByte);


const uint8_t DisplayUpdateRate = 1; //*100

float CycleTime = 0;
long CycleCounter = 0;

int rec_msg_counter = 0;
int RSSI = 0;
int LQI = 0;
int loss = 0;
uint16_t senderBattVolt;
uint8_t senderID;

void setup(){
    Serial.begin(9600);
    DisplaySetup();
    RadioSetup();
}
void loop(){
    static uint64_t LastMsg = 0;
    uint8_t lowByte;
    uint8_t highByte;
    byte buffer[3] = {0};
    int len;
    //Checks whether something has been received.
    //When something is received we give some time to receive the message in full.(time in millis)
    if (ELECHOUSE_cc1101.CheckRxFifo(100)){
        if (ELECHOUSE_cc1101.CheckCRC()){    //CRC Check. If "setCrc(false)" crc returns always OK!
        rec_msg_counter++;
        RSSI  = ELECHOUSE_cc1101.getRssi();    
        LQI   = ELECHOUSE_cc1101.getLqi();
        len   = ELECHOUSE_cc1101.ReceiveData(buffer);

        Serial.print("T:");Serial.print(millis()/1000);
        Serial.print("  Rssi: ");
        
        Serial.print(RSSI);
        Serial.print("  LQI: ");
        
        Serial.print(LQI);
        Serial.print("  Message: ");
        
        //buffer[len] = '\0';
        
        Serial.println((char *) buffer);
        lowByte = buffer[0];
        highByte = buffer[1];
        senderID = buffer[2];
        
        senderBattVolt = combineFromBytes( highByte, lowByte);

        LastMsg = millis();
        }
    }
    if (millis() - LastMsg > 1000 ){
      RSSI      = 0;
      LQI       = 0;
      lowByte   = 0;
      highByte  = 0;
      senderID  = 0;
    }        
    DisplayUpdater();
}
void DisplaySetup(){
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0,"Hello World!");
    u8x8.clear();
}
void RadioSetup(){
  if (ELECHOUSE_cc1101.getCC1101()){         // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
  }else{
    Serial.println("Connection Error");
  }
    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setCrc(1);      // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    
    Serial.println("Rx Mode");
}
void DisplayUpdater(){
  static uint64_t previousMillis_DisplayUpdater = 0;

  if (millis() - previousMillis_DisplayUpdater >= DisplayUpdateRate*100) {
      previousMillis_DisplayUpdater = millis();

      u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);

      u8x8.setCursor(0, 1);
      u8x8.print("Sen ID: ");u8x8.print(senderID);   
      u8x8.setCursor(0, 2);
      u8x8.print("Sen batt: "); u8x8.print(senderBattVolt/1000.0,2);  u8x8.print("V");
      
      u8x8.setCursor(0, 4);
      u8x8.print("# msg rec:"); u8x8.print(rec_msg_counter);
      u8x8.setCursor(0, 5);
      u8x8.print("RSSI: "); u8x8.print(RSSI); u8x8.print("  ");
      u8x8.setCursor(0, 6);
      u8x8.print("LQI: "); u8x8.print(LQI); u8x8.print("  ");
      u8x8.setCursor(0, 7);
      u8x8.print("CPM: "); u8x8.print(LQI); u8x8.print("  ");
  }
}
uint16_t combineFromBytes(uint8_t highByte, uint8_t lowByte){
   return ((uint16_t)highByte << 8) | lowByte;
}