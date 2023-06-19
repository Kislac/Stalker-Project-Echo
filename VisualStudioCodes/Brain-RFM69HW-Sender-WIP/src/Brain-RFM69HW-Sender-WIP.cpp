/**
 * @file RFM69-Node-STM32.ino
 * 
 * @author     Jonas Scharpf <jonas@brainelectronics.de>
 * @date       July 2018
 * @version    1.0
 * 
 * @brief      Sample RFM69 sender/node sketch, with ACK and optional 
 *             encryption, and Automatic Transmission Control driven by a 
 *             STM32F103C8 or similar
 * 
 * Description:
 *   This sketch has been successfully tested on a STM32F103C8 bluepill.
 *   
 *   It sends periodic messages of increasing length to gateway (id = 1)
 *   The code has a 10sec startup delay, due to the
 *   missing wait-until-USB-connected function of the STM32.
 *   The default LED is active during the setup function.
 *   The sketch prints out the file name as well as compilation date and time
 *  
 *  
 * Limits (At the date of creation):
 *    None
 *
 *
 * Note:
 *  for further understandings of the code, please read the doxygen docs
 *  also consider taking a look at the following code snippets/examples
 *  - None
 * 
 * 
 * TBD:
 *  see issues at the git
 * 
 * 
 * Improvements:
 *  None
 *  
 * 
 * Versions:
 *    1.0:
 *      set and get CS pin, IRQ pin, get power level
 *      
 *    Original RFM69 library and sample code by Felix Rusu
 *    http://LowPowerLab.com/contact
 *    Copyright Felix Rusu (2015)
 *  
 *  
 * The circuit:
 *  STM32F103C Bluepill Board
 *  RFM69 attached to SPI_1 
 *  CS    <-->  PA4 <-->  BOARD_SPI1_NSS_PIN
 *  SCK   <-->  PA5 <-->  BOARD_SPI1_SCK_PIN
 *  MISO  <-->  PA6 <-->  BOARD_SPI1_MISO_PIN
 *  MOSI  <-->  PA7 <-->  BOARD_SPI1_MOSI_PIN
 *  DIO0  <-->  PA8 <-->  Interrupt on receiving message
 * 
 * 
 * RFM69 library and sample code by Felix Rusu - http://LowPowerLab.com/contact
 * Copyright Felix Rusu (2015)
 * 
 * 
 ***************************************************************************/

//----Includes----Start****************************************************************
#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <FlashStorage_STM32.h>
#include <StalkerProtocol.h>
//--RFM69 inlcudes--start

// include all libs
#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>//get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>      //comes with Arduino IDE (www.arduino.cc)
// #include <SPIFlash.h> //get it here: https://www.github.com/lowpowerlab/spiflash

//--RFM69 inlcudes--end

//----Includes----END***********************************************************************
/*
Oled display wiring (SH1106_128X64)
SSH1106   - STM32 (Bluepill)
VCC       - 3,3V (5v tolerant)
GND       - GND
SCL       - PB6 - SCL1
SDA       - PB7 - SDA1
*/
//U8G2_SH1106_128X32_VISIONOX_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  
//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
//
#define NODEID        5    //Ki vagyok én? must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     100  //the same on all nodes that talk to each other (range up to 255)
#define GATEWAYID     255    //Kinek küldöm? //255 Broadcast
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY   RF69_433MHZ
//#define FREQUENCY   RF69_868MHZ
//#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
//#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//*********************************************************************************************


//----Defines----Start
  #define RFM69_RST     PA2   // GPIO02/D4
  #define active         LOW
  #define inactive       HIGH
  #define onboardLED     PC13
  //#define iwdg_init_ms(N) iwdg_init(IWDG_PRE_256,((N)/5))
  #ifdef ENABLE_ATC
    RFM69_ATC radio;
  #else
    //RFM69 radio;
    //RFM69 radio = RFM69(PA4, PA3, true, PA3);
    RFM69 radio = RFM69(PA8, PA3, true, PA3);
    //RFM69 radio = RFM69(PA15, PA8, true, PA8);
  #endif
  //Stalker stalker;
//----Defines----End

//----Declarations----Start
int TRANSMITPERIOD = 1000; //transmit a packet to gateway so often (in ms)
char payload[] = "123-ABCDEFG";
char buff[20];
char SentMsg[64];
int SendingMsg = 0;
uint32_t packetCount = 0;
byte sendSize = 0;
//boolean requestACK = true;
const int SerialID = 115200;
long lastPeriod = 0;
bool promiscuousMode = false; //set to 'true' to sniff all packets on the same network
unsigned long previousMillis_LoadingIcon = 0;
unsigned long currentMillis = millis();
int IconState = 0;
bool RFM_Recive_msg_DispFlag = false;
bool RFM_Send_msg_DispFlag = false;
char IncomingMsg[64];
int CurrentRSSI = 0;
uint8_t IntegerDeconstruct_Length=0;
uint8_t IntegerDeconstruct_Output[4];


//----Declarations----End


//----Funcitions----Start
void Blink(byte PIN, int DELAY_MS);
void ProcessSerialInput();
void RFM_Recive_msg();
void RFM_Send_msg();
void AliveAnimation();
void DisplayUpdater();
//void StalkerProtocolTest();
//void StalkerProtocol_123_FromTo_id();
void IntegerDeconstruct_4ByteMax(int Input);
void BuiltPreMSG(uint32_t From_id, uint32_t To_id, uint8_t CurrentMsg, uint8_t TotalMsg, uint8_t DevType, char name[]);
void SendStalkerProtocolMsg(int Interval);
void AddCommand(uint8_t Command, uint32_t CommandData);
//----Functions----End



//StalkerProtocol
//int SP_ID = 1;
uint32_t Device_id = 999999;
uint8_t DeviceType = 13;
char DeviceName[]= "NoName";
uint8_t MsgPayload[61];
uint8_t MsgPayloadByteCount = 0;

//StalkerProtocol

// the setup function runs once when you press reset or power the board
void setup(){
  Serial.begin(SerialID);     // USB:           PA11 (DM), PA12 (DP)
  // Serial1.begin(9600);    // PA9 (STM-TX), PA10 (STM-RX)
  // Serial2.begin(9600);    // PA2 (STM-TX), PA3 (STM-RX)
  // Serial3.begin(9600);    // PB10 (STM-TX), PB11 (STM-RX)
  
  delay(500);
  while (!Serial) 
    ;
  Serial.println("");
  Serial.print("Serial.begin:");Serial.println(SerialID);

 //RESET RFM69 module
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);  Serial.println("RFM69_RST - HIGH");  delay(100);
  digitalWrite(RFM69_RST, LOW);  Serial.println("RFM69_RST - LOW");  delay(100);
 //RESET RFM69 module


  //U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

  



//INIT u8g2
  u8g2.begin();  
  u8g2.enableUTF8Print();
  

  //u8x8.begin();
  //u8x8.setPowerSave(0);
//INIT u8g2


  // define onboard led pin as output and activate it
  pinMode(onboardLED, OUTPUT);
  digitalWrite(onboardLED, active);

  // set the slave select (CS) pin (PA4 is default for SPI_1)
  //radio.setSlaveSelectPin(PA4);
  // get the IRQ pin (which is connected to RFM69's DIO0)
  //radio.setInterruptPin(PA3);
  // set the IRQ pin number (at the STM32 must be same as interrupt pin)
  //radio.setInterruptNumber(radio.getInterruptPin());
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
//radio.setPowerLevel(1);
#ifdef IS_RFM69HW
  //radio.setHighPower(); // uncomment only for RFM69HW!
  radio.setPowerLevel(31);
#endif
  //radio.encrypt(ENCRYPTKEY);
  radio.encrypt(null);
  radio.promiscuous(promiscuousMode);
  //radio.setFrequency(919000000); // set frequency to some custom frequency

//Auto Transmission Control - dials down transmit power to save battery (-100 is the noise floor, -90 is still pretty good)
//For indoor nodes that are pretty static and at pretty stable temperatures (like a MotionMote) -90dBm is quite safe
//For more variable nodes that can expect to move or experience larger temp drifts a lower margin like -70 to -80 would probably be better
//Always test your ATC mote in the edge cases in your own environment to ensure ATC will perform as you expect
#ifdef ENABLE_ATC
  radio.enableAutoPower(-70);
#endif

  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);


  
#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)\n");
#endif

  // report successfull performed setup
  Serial.println("Setup done, starting soon...");

  // turn onboard led off
  digitalWrite(onboardLED, inactive);


  Serial.print(F("\nStart EEPROM_write on ")); Serial.println(BOARD_NAME);
  Serial.println(FLASH_STORAGE_STM32_VERSION);

  Serial.print("EEPROM length: ");
  Serial.println(EEPROM.length());

}


// the loop function runs over and over again forever
void loop()
{
//radio.setPowerLevel(1);
  currentMillis = millis(); 

  ProcessSerialInput();
  RFM_Recive_msg();
  RFM_Send_msg();

  DisplayUpdater();

  //StalkerProtocolTest();
  //SendStalkerProtocolMsg(500);
  

  //StalkerProtocol_123_FromTo_id();


}

void SendStalkerProtocolMsg(int Interval){
  int currPeriod = millis()/Interval;
    if (currPeriod != lastPeriod)
  {
    lastPeriod = currPeriod;

    BuiltPreMSG(Device_id,666666,1,1,DeviceType,DeviceName);
    AddCommand(Command_06_Alpha_Rad_RO,0);
    AddCommand(Command_07_Beta_Rad_RS,11);
    AddCommand(Command_08_Beta_Rad_RO,22);
    AddCommand(Command_09_Gamma_Rad_RS,33);
    AddCommand(Command_10_Gamma_Rad_RO,33);
    AddCommand(Command_11_Beta_Rad_RS,33);
    AddCommand(Command_12_Beta_Rad_RO,33);
    AddCommand(Command_13_Theta_Rad_RS,33);
    AddCommand(Command_14_Theta_Rad_RO,33);
    AddCommand(Command_15_PhsziStrenght,33);
    AddCommand(Command_16_CurrentRadLevel,33);
    //AddCommand(Command_17_GPS_N,35706100);
    //AddCommand(Command_18_GPS_E,51391670);


        Serial.print("Sent: ");
    radio.send(GATEWAYID, MsgPayload, MsgPayloadByteCount,false);
    for(int i = 0; i<MsgPayloadByteCount;i++){
        if((MsgPayload[i] & 0x80) == 0x80){ //0x80=0b 1000 0000=128
          Serial.printf("[%d]|", (MsgPayload[i] & 0x7F));
          //Serial.print("[");Serial.print(MsgPayload[i] & 0x7F);Serial.print("]");Serial.print("|");
        }else{
          Serial.printf("%d|", (MsgPayload[i]));
          //Serial.print(MsgPayload[i]);Serial.print("|");
        }
    }
    Serial.printf(" Length: %d", MsgPayloadByteCount);
    Serial.println("");


  }
}

void AddCommand(uint8_t Command, uint32_t CommandData){
  MsgPayload[MsgPayloadByteCount] = (byte) (Command | 0x80);  MsgPayloadByteCount++;
  IntegerDeconstruct_4ByteMax(CommandData);
  for(int i = 0; i<IntegerDeconstruct_Length; i++){
    MsgPayload[MsgPayloadByteCount+i]=IntegerDeconstruct_Output[i];
    MsgPayloadByteCount++;
  }

}

void BuiltPreMSG(uint32_t From_id, uint32_t To_id, uint8_t CurrentMsg, uint8_t TotalMsg, uint8_t DevType, char name[]){

  MsgPayloadByteCount = 0;
  MsgPayload[MsgPayloadByteCount] = (byte) (Command_00_From_id | 0x80);  MsgPayloadByteCount++;
  IntegerDeconstruct_4ByteMax(From_id);
  //Serial.printf("IntegerDeconstruct_Length: %d\n",IntegerDeconstruct_Length);
  for(int i = 0; i<IntegerDeconstruct_Length; i++){
    MsgPayload[MsgPayloadByteCount]=IntegerDeconstruct_Output[i];
    //Serial.printf("MsgPayloadByteCount: %d, i: %d, data: %d\n", MsgPayloadByteCount, i,MsgPayload[MsgPayloadByteCount+i]);
    MsgPayloadByteCount++;
  }

    MsgPayload[MsgPayloadByteCount] = (byte) (Command_01_To_id | 0x80);  MsgPayloadByteCount++;
  IntegerDeconstruct_4ByteMax(To_id);
  for(int i = 0; i<IntegerDeconstruct_Length; i++){
    MsgPayload[MsgPayloadByteCount]=IntegerDeconstruct_Output[i];
    MsgPayloadByteCount++;
  }

    MsgPayload[MsgPayloadByteCount] = (byte) (Command_02_CurrMsg | 0x80);  MsgPayloadByteCount++;
  IntegerDeconstruct_4ByteMax(CurrentMsg);
  for(int i = 0; i<IntegerDeconstruct_Length; i++){
    MsgPayload[MsgPayloadByteCount]=IntegerDeconstruct_Output[i];
    MsgPayloadByteCount++;
  }

      MsgPayload[MsgPayloadByteCount] = (byte) (Command_03_TotalMsg | 0x80);  MsgPayloadByteCount++;
  IntegerDeconstruct_4ByteMax(TotalMsg);
  for(int i = 0; i<IntegerDeconstruct_Length; i++){
    MsgPayload[MsgPayloadByteCount]=IntegerDeconstruct_Output[i];
    MsgPayloadByteCount++;
  }

    MsgPayload[MsgPayloadByteCount] = (byte) (Command_04_Device_Type | 0x80);  MsgPayloadByteCount++;
  IntegerDeconstruct_4ByteMax(DevType);
  for(int i = 0; i<IntegerDeconstruct_Length; i++){
    MsgPayload[MsgPayloadByteCount]=IntegerDeconstruct_Output[i];
    MsgPayloadByteCount++;
  }

    MsgPayload[MsgPayloadByteCount] = (byte) (Command_05_Device_Name | 0x80);  MsgPayloadByteCount++;

  for (int i = 0; i < sizeof(DeviceName)-1;i++){
    MsgPayload[MsgPayloadByteCount] = DeviceName[i];
    MsgPayloadByteCount++;
    //Serial.printf("%c",DeviceName[i]);
  }
  //Serial.printf("sizeof(DeviceName): %d\n", sizeof(DeviceName));
  //Serial.printf("sizeof(DeviceName): %d\n", sizeof(DeviceName));


}



void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, active);
  delay(DELAY_MS);
  digitalWrite(PIN, inactive);
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
    if (input == 'p')
    {
      promiscuousMode = !promiscuousMode;
      radio.promiscuous(promiscuousMode);
      Serial.print("Promiscuous mode ");Serial.println(promiscuousMode ? "on" : "off");
    }      
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
      
      Serial.print("Slave Select Pin: ");
      Serial.println(radio.getSlaveSelectPin());
      Serial.print("Interrupt Pin: ");
      Serial.println(radio.getInterruptPin());
      Serial.print("Interrupt Number: ");
      Serial.println(radio.getInterruptNumber());
      Serial.print("Is RFM69HW: ");
      Serial.println(radio.getHighPower());
      Serial.print("Power Level: ");
      Serial.println(radio.getPowerLevel());
      Serial.print("Frequency: ");
      Serial.println(radio.getFrequency());
      Serial.print("NODEID: ");
      Serial.println(radio.getAdress());
      Serial.print("NETWORKID: ");
      Serial.println(radio.getNetwork());

      Serial.print("Test Pin (LED on STM32F103C Bluepill): ");
      Serial.println(radio.getTestPin());
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

void RFM_Recive_msg(){
  RFM_Recive_msg_DispFlag = true;

    if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);
    Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
    if (promiscuousMode)
      {
        Serial.print("to [");Serial.print(radio.TARGETID, DEC);Serial.print("] ");
      }
    packetCount++;
    for(int b = 0; b < sizeof(IncomingMsg);  ++b )
        IncomingMsg[b] = (char)0;

    //memset(IncomingMsg, 0, sizeof(IncomingMsg)); 
      for (byte i = 0; i < radio.DATALEN; i++){
        Serial.print((char)radio.DATA[i]);
        IncomingMsg[i] = (char)radio.DATA[i];
      }
    //IncomingMsg_INT = atoi(IncomingMsg);
      Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
      //Serial.print("   Int:");Serial.print(IncomingMsg_INT);
      CurrentRSSI = radio.RSSI;



    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    Blink(onboardLED, 3);
    Serial.println();
  }
}

void RFM_Send_msg(){
  RFM_Send_msg_DispFlag = true;
  int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    lastPeriod = currPeriod;

    for(int b = 0; b < sizeof(SentMsg);  ++b )
        SentMsg[b] = (char)0;

    Serial.print("Sending[");
    Serial.print(sendSize);
    Serial.print("]: ");


    for(byte i = 0; i < sendSize; i++){
      Serial.print((char)payload[i]);
      SentMsg[i] = (char)payload[i];
    }
      

    sendSize = (sendSize + 1) % 12;
    radio.send(GATEWAYID, payload, sendSize,false);
    /*if (radio.sendWithRetry(GATEWAYID, payload, sendSize))
    {
     Serial.print(" ok!");
    }   
    else
    {
     Serial.print(" nothing...");
    }*/
    

    
/*
    SendingMsg++;
    Serial.print(SendingMsg);
  
    sprintf(payload, "%d", SendingMsg);
    Serial.print("   strlen(payload):");Serial.print(strlen(payload));
    sendSize = 1;
*/


    //radio.send(GATEWAYID, payload, sendSize,false);
    
    Serial.println();
    Blink(onboardLED, 3);
  }
}

void AliveAnimation(){


  if (currentMillis - previousMillis_LoadingIcon >= 80) {
    // save the last time you blinked the LED
    previousMillis_LoadingIcon = currentMillis;
    IconState++;
    if(IconState == 8){
      IconState = 0;
    }
    //Serial.print("IconState:");Serial.println(IconState);
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
  if (RFM_Recive_msg_DispFlag == true){
    u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
    u8g2.setCursor(0,7); 
    u8g2.print("RSSI: ");u8g2.print(CurrentRSSI);
    u8g2.setCursor(0,14); 
    u8g2.print("Got:[");u8g2.print(packetCount);u8g2.print(']');u8g2.print(IncomingMsg);
  }
  if (RFM_Send_msg_DispFlag == true){
    u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
    u8g2.setCursor(0,21); 
    u8g2.print("Sent: ");u8g2.print(SentMsg); 

  }

  u8g2.sendBuffer();          // transfer internal memory to the display
}





void IntegerDeconstruct_4ByteMax(int InputInt){

  
    //Serial.printf("InputInt: %d\n", InputInt);
    IntegerDeconstruct_Output[0] = (byte) (InputInt & 0x7F);       //0x7F=0b 0111 1111=127
    IntegerDeconstruct_Output[1] = (byte) (InputInt >> 7) & 0x7F;  //0x7F=0b 0111 1111=127
    IntegerDeconstruct_Output[2] = (byte) (InputInt >> 14) & 0x7F; //0x7F=0b 0111 1111=127
    IntegerDeconstruct_Output[3] = (byte) (InputInt >> 21) & 0x7F; //0x7F=0b 0111 1111=127

  //for (int i = 0; i<4;i++){
  //   Serial.printf("data[%d]: %d\n", i, IntegerDeconstruct_Output[i]);
  // }



    for (int i = 3; i>=0;i--)
    {
      if(IntegerDeconstruct_Output[i] != 0){
        IntegerDeconstruct_Length = i+1;
        i=-1;
      }
    } 
    
    // Serial.printf("IntegerDeconstruct_Length: %d\n", IntegerDeconstruct_Length);
    //for (int i = 0; i<IntegerDeconstruct_Length;i++){
    // Serial.printf("data[%d]: %d\n", i, IntegerDeconstruct_Output[i]);
    //}
    
    return;
}




















