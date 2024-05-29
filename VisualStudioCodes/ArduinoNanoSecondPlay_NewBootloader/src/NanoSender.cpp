////pio device monitor -p COM4 -b 9600
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
//  Serial.println("Nano1---Start--");
//  Serial.println();
//}
//
//// the loop function runs over and over again forever
//void loop() {
//  states = !states;
//  digitalWrite(LED_BUILTIN, states);  // turn the LED on (HIGH is the voltage level)
//  Serial.print("Nano1-S# ");Serial.println(states);
//  delay(100);                      // wait for a second
//                  // wait for a second
//}

//New transmission method.
//In addition, the gdo0 and gdo2 pin are not required.
//https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
//by Little_S@tan
//#include <Bonezegei_Printf.h>
//Bonezegei_Printf debug(&Serial);
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Arduino.h>
//#include <U8g2lib.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


byte transmitt_byte[11] = {72,101,108,108,111,32,87,111,114,108,100};
char *transmitt_char = "Hello World";
int num =0;

void AliveAnimation();
void DisplayUpdater();

unsigned long previousMillis_LoadingIcon = 0;
unsigned long previousMillis_Transmitperiod = 0;
unsigned long previousMillis_CycleTime = 0;
unsigned long previousMillis_DisplayUpdater = 0;
uint8_t DisplayUpdateRate = 10; //*100
uint8_t AliveAnimationRate = 5; //*100
uint8_t TransmitPeriod = 1000; //*100
uint8_t IconState;
float CycleTime = 0;
long CycleCounter = 0;
int time_sec = 5;

void setup() {
    //u8g2.begin();
    //u8g2.enableUTF8Print();
    //u8g2.clearBuffer();					// clear the internal memory
    //u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    //u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
    //u8g2.sendBuffer();					// transfer internal memory to the display
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0,"Hello World!");
    u8x8.clear();
    //u8x8.setCursor(2,0); 
    //u8x8.print("MS<=X=>PC");
    //u8x8.drawString(0,1,"Hello World!");

    Serial.begin(9600);
    Serial.println();
    Serial.println("Nano1-Sender---Start--");
    Serial.println();
    if (ELECHOUSE_cc1101.getCC1101()){        // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
    }else{
    Serial.println("Connection Error");
    }
 
    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
 // ELECHOUSE_cc1101.setPA(10);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setCrc(1);     // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.

    Serial.println("Tx Mode");
}

void loop() {


if (millis() - previousMillis_Transmitperiod >= 110) {
    // save the last time you blinked the LED
    previousMillis_Transmitperiod = millis();
//Transmitt "Hello World" from byte format.
ELECHOUSE_cc1101.SendData(transmitt_byte, 11, 100);
num++;
//Serial.print("Nano1-Sent# ");Serial.println(num);
}


if (millis() - previousMillis_CycleTime >= 1000) {
  previousMillis_CycleTime = millis();
  CycleTime = 1000.0/CycleCounter;
  //Serial.print("CycleTime: ");Serial.print(CycleTime,3);
  //Serial.print("ms; CycleCounter: ");Serial.println(CycleCounter);
  CycleCounter = 0;
}
CycleCounter++;
////Transmitt "Hello World" from char format.
//ELECHOUSE_cc1101.SendData(transmitt_char, 100);
//delay(2000);
//Serial.print("Nano1-Sent# ");Serial.println(num++);
////Transmitt "Hello World" from char format directly.
//ELECHOUSE_cc1101.SendData("Hello World", 100);
//delay(2000);
//Serial.print("Nano1-Sent# ");Serial.println(num++);


DisplayUpdater();

  //u8x8.drawString(2,2,"Alma!");


}

void DisplayUpdater(){
  if (millis() - previousMillis_DisplayUpdater >= DisplayUpdateRate*100) {
      previousMillis_DisplayUpdater = millis();

      //u8g2.clearBuffer(); // clear the internal memory
      //time_sec++;

      //AliveAnimation();
      float time_sec2 = millis()/1000.0;
      //u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
      //u8g2.setCursor(0,38);u8g2.print("Time: "); 
      //u8g2.print(time_sec); 
      //u8g2.setCursor(0,48);u8g2.print("sent:");
      //u8g2.sendBuffer();

      u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
      u8x8.setCursor(0, 0);
      u8x8.print(time_sec2,1);u8x8.print("s");   
      u8x8.setCursor(0, 1);
      u8x8.print(CycleTime,3); u8x8.print("ms");  
      u8x8.setCursor(0, 3);
      u8x8.print("Sent: #"); u8x8.print(num); 
      //u8x8.setCursor(4,4);u8x8.print("time_sec");
      //u8x8.setCursor(3,3);
      //u8x8.print(time_sec);
      //u8x8.drawString(2,2,"Hello World!");
      //u8x8.drawString(3,3,"time_sec");

  }
}

//void AliveAnimation(){
//
//
//  if (millis() - previousMillis_LoadingIcon >= AliveAnimationRate*100) {
//    // save the last time you blinked the LED
//    previousMillis_LoadingIcon = millis();
//    IconState++;
//    if(IconState == 8){
//      IconState = 0;
//    }
//  }
//
//
//  u8g2.setDrawColor(1);
//
//  switch (IconState){
//    case 0:
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      break;
//
//    case 1:
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      break;
//    case 2:
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      break;
//    case 3:
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      u8g2.setDrawColor(0);
//      break;
//
//
//
//    case 4:
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      break;
//    case 5:
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      break;
//    case 6:
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.setDrawColor(1);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      break;
//    case 7:
//      u8g2.setDrawColor(0);
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_RIGHT );
//      
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_UPPER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_LEFT );
//      u8g2.drawCircle(123, 4, 4,U8G2_DRAW_LOWER_RIGHT );
//      u8g2.setDrawColor(1);
//      break;
//        
//    }
//    
//  u8g2.setDrawColor(1);
//}