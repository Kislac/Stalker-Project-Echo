////pio device monitor -p COM5 -b 9600
//#include <Arduino.h>
////#include <stdio.h>
////#include <Wire.h>
//// put function declarations here:
//int myFunction(int, int);
//boolean states =0;
//
//
//
//void setup() {
//  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(9600);
//  //Serial.printf("\nNano1---Start--\n");
//  Serial.println();
//  Serial.println("Nano2---Start--");
//  Serial.println();
//}
//
//// the loop function runs over and over again forever
//void loop() {
//  states = !states;
//  digitalWrite(LED_BUILTIN, states);  // turn the LED on (HIGH is the voltage level)
//  Serial.print("Nano2-R# ");Serial.println(states);
//  delay(1000);                      // wait for a second
//                  // wait for a second
//}



//New receiving method. This method checks the Rx Fifo for any data it contains.
//It allows you to do several things in a loop.
//In addition, the gdo0 and gdo2 pin are not required.
//https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
//by Little_S@tan
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Arduino.h>
//#include <U8g2lib.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


void DisplayUpdater();

unsigned long previousMillis_LoadingIcon = 0;
unsigned long previousMillis_Transmitperiod = 0;
unsigned long previousMillis_CycleTime = 0;
unsigned long previousMillis_DisplayUpdater = 0;
unsigned long LastMsg = 0;
uint8_t DisplayUpdateRate = 1; //*100
uint8_t AliveAnimationRate = 5; //*100
uint8_t TransmitPeriod = 1000; //*100
uint8_t IconState;
float CycleTime = 0;
long CycleCounter = 0;
int time_sec = 5;
int rec_msg_counter = 0;
int RSSI = 0;
int LQI = 0;
int loss = 0;
void setup(){

    Serial.begin(9600);
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0,"Hello World!");
    u8x8.clear();

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
byte buffer[61] = {0};

void loop(){

    //Checks whether something has been received.
    //When something is received we give some time to receive the message in full.(time in millis)
    if (ELECHOUSE_cc1101.CheckRxFifo(100)){
        if (ELECHOUSE_cc1101.CheckCRC()){    //CRC Check. If "setCrc(false)" crc returns always OK!
        rec_msg_counter++;
        Serial.print("T:");Serial.print(millis()/1000);
        Serial.print("  Rssi: ");
        RSSI = ELECHOUSE_cc1101.getRssi();
        Serial.print(RSSI);
        Serial.print("  LQI: ");
        LQI = ELECHOUSE_cc1101.getLqi();
        Serial.print(LQI);
        Serial.print("  Message: ");
        int len = ELECHOUSE_cc1101.ReceiveData(buffer);
        buffer[len] = '\0';
        Serial.println((char *) buffer);
        LastMsg = millis();
        //for (int i = 0; i<len; i++){
        //Serial.print(buffer[i]);
        //Serial.print(",");
        //}
        //Serial.println();
        }
    }


    if (millis() - LastMsg >= 600) {
        loss++;
        LastMsg = millis();
    }



    if (millis() - previousMillis_CycleTime >= 1000) {
    previousMillis_CycleTime = millis();
    CycleTime = 1000.0/CycleCounter;
    Serial.print("CycleTime: ");Serial.print(CycleTime,3);
    Serial.print("ms; CycleCounter: ");Serial.println(CycleCounter);
    CycleCounter = 0;
    }
    CycleCounter++;

    DisplayUpdater();
}

void DisplayUpdater(){
  if (millis() - previousMillis_DisplayUpdater >= DisplayUpdateRate*100) {
      previousMillis_DisplayUpdater = millis();

      float time_sec2 = millis()/1000.0;


      u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
      u8x8.setCursor(0, 0);
      u8x8.print(time_sec2,1);u8x8.print("s");   
      u8x8.setCursor(0, 1);
      u8x8.print(CycleTime,3); u8x8.print("ms");  
      u8x8.setCursor(0, 3);
      u8x8.print("#"); u8x8.print(rec_msg_counter); u8x8.print("/"); u8x8.print(rec_msg_counter+loss);
      u8x8.setCursor(0, 4);
      u8x8.print("RSSI: "); u8x8.print(RSSI); u8x8.print("  ");
      u8x8.setCursor(0, 5);
      u8x8.print("LQI: "); u8x8.print(LQI); u8x8.print("  ");

      u8x8.setCursor(0, 7);
      float perc = 100.0*loss / (rec_msg_counter+loss);
      u8x8.print("loss:"); 
      u8x8.print(loss); 
      u8x8.print("-"); 
      u8x8.print(perc,2);
      u8x8.print("%        ");


  }
}