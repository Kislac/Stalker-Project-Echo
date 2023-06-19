/**
 * @file RFM69-Node-STM32.ino
 * 
 * @author     Jonas Scharpf <jonas@brainelectronics.de>
 * @date       July 2018
 * @version    1.0
 * 
 * @brief      Sample RFM69 receiver/gateway sketch, with ACK 
 *             and optional encryption, and Automatic Transmission Control
 *             Passes through any wireless received messages to the serial port
 *             and responds to ACKs. Module is driven by a STM32F103C8 or similar
 * 
 * Description:
 *   This sketch has been successfully tested on a STM32F103C8 bluepill.
 *   
 *   It receives the periodic messages of increasing length from a node
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
 *  CS(NSS) <-->  PA4 <-->  BOARD_SPI1_NSS_PIN
 *  SCK     <-->  PA5 <-->  BOARD_SPI1_SCK_PIN
 *  MISO    <-->  PA6 <-->  BOARD_SPI1_MISO_PIN
 *  MOSI    <-->  PA7 <-->  BOARD_SPI1_MOSI_PIN
 *  DIO0    <-->  PA3 <-->  Interrupt on receiving message
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
#define NODEID        8    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define GATEWAYID     255  //255 Broadcast
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_433MHZ
//#define FREQUENCY     RF69_868MHZ
// #define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
//#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//*********************************************************************************************



// the setup function runs once when you press reset or power the board

//----Defines----Start
#define RFM69_RST     PA2   // GPIO02/D4
#define active         LOW
#define inactive       HIGH
#define onboardLED     PC13
#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  //RFM69 radio;
  RFM69 radio = RFM69(PA4, PA3, true, PA3);
#endif
/*
#define LED_BUILTIN PC13
#define Buzzer PB1
#define analogInPin PA0*/
//----Defines----End

//----Declarations----Start
bool promiscuousMode = false; //set to 'true' to sniff all packets on the same network
bool SzeretlekState = true;
const int SerialID = 115200;
byte ackCount=0;
uint32_t packetCount = 0;
unsigned long previousMillis_LoadingIcon = 0;
unsigned long  previousMillis_SzeretlekIcon = 0;
unsigned long currentMillis = millis();
int IconState = 0;
char IncomingMsg_char[230];
int IncomingMSG_Lenght;
int CurrentRSSI = 0;
int TRANSMITPERIOD = 500; //transmit a packet to gateway so often (in ms)
int IncomingMsg_int[230];
int prevMsg;
int MissedMsg = 0;
long lastPeriod = 0;
char SentMsg[230];
byte sendSize = 0;
char payload[] = "Almafa";
bool RFM_Recive_msg_DispFlag = false;
bool RFM_Send_msg_DispFlag = false;
/*
const int SerialID = 115200;
const int ledPin =  LED_BUILTIN;
int ledState = LOW; 
unsigned long previousMillis = 0;
const long interval = 1000;

unsigned long previousMillis_Tone = 0;
unsigned long interval_Tone = 3000;
unsigned long previousMillis_CycleTime = 0;
unsigned long previousMillis_LoadingIcon = 0;
unsigned long currentMillis = millis();
int IconState = 0;
char buf2[4];
int CycleTime = 5;
int CycleCounter;
char buf[255];
int GeigerProg_prev;
int CycleTime_prev;
char CurrentButton;
bool BuzzerEnable = false;
uint32_t lastrange = 3000;*/

char StalkerCommandList [19][100]=
{
  "00-From_id:",
  "01-To_id:",
  "02-CurrMsg:",
  "03-TotalMsg:",
  "04-DeviceType:",
  "05-DeviceName:",
  "06-Alpha_Rad_RO",  
  "07-Beta_Rad_RS",    
  "08-Beta_Rad_RO", 
  "09-Gamma_Rad_RS", 
  "10-Gamma_Rad_RO", 
  "11-Beta_Rad_RS", 
  "12-Beta_Rad_RO", 
  "13-Theta_Rad_RS", 
  "14-Theta_Rad_RO", 
  "15-PhsziStrenght", 
  "16-CurrentRadLevel", 
  "17-GPS_N", 
  "18-GPS_E",

};

//----Declarations----End

//----Funcitions----Start
void Blink(byte PIN, int DELAY_MS);
void ProcessSerialInput();
void RFM_Recive_msg();
void RFM_Send_msg();
void DrawIcon();
void AliveAnimation();
void DisplayUpdater();
void RunCommand(uint8_t command, uint8_t commandData_Count, uint32_t commandData[60]);
int IntegerReconstruct_4ByteMax(uint32_t InputInt[]);
int* IntegerSplit_4ByteMax(uint32_t InputInt);
void SplitAndBuiltInteger();
void DecodeIncomingMsg();
//----Functions----End


void setup()
{
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

#ifdef IS_RFM69HW
  radio.setHighPower(); // uncomment only for RFM69HW!
#endif
  //radio.encrypt(ENCRYPTKEY);
  radio.encrypt(null);
  radio.promiscuous(promiscuousMode);
  //radio.setFrequency(919000000); //set frequency to some custom frequency
  
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);



#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)");
#endif

  // report successfull performed setup
  Serial.println("Setup done, starting soon...");

  // turn onboard led off
  digitalWrite(onboardLED, inactive);
}


// the loop function runs over and over again forever
void loop()
{
  
  currentMillis = millis(); 

  ProcessSerialInput();
  RFM_Recive_msg();
  //RFM_Send_msg();
  //DrawIcon();
  //SplitAndBuiltInteger();
  DisplayUpdater();
  

  
}

int IntegerReconstruct_4ByteMax(uint32_t InputInt[], uint8_t length){
  uint32_t Outputint = 0;
  if (length > 4){
    length = 4;
  }
  for (uint8_t i = 0; i < length; i++ ){
    //Serial.print("i: ");Serial.print(i);Serial.print(" | InputInt[length-i-1]: ");Serial.print(InputInt[length-i-1]);
    Outputint = Outputint+(InputInt[length-i-1] << ((length-i-1)*7));
//3
  }
  //Serial.println("");
  //Serial.printf("Outputint3: %d\n", Outputint);
  //Serial.print("Outputint3: "); Serial.println(Outputint);

  //Outputint = (InputInt[3] << 21) + (InputInt[2] << 14) + (InputInt[1] << 7) + (InputInt[0]);// + (data[2] << 8) + (data[3] << 8);
  return Outputint;

}

int* IntegerSplit_4ByteMax(int InputInt){
  static int OutputintArray[4];
    OutputintArray[0] = (byte) (InputInt & 0x7F);       //0x7F=0b 0111 1111=127
    OutputintArray[1] = (byte) (InputInt >> 7) & 0x7F;  //0x7F=0b 0111 1111=127
    OutputintArray[2] = (byte) (InputInt >> 14) & 0x7F; //0x7F=0b 0111 1111=127
    OutputintArray[3] = (byte) (InputInt >> 21) & 0x7F; //0x7F=0b 0111 1111=127

    Serial.printf("data[0]: %d\n", OutputintArray[0]);
    Serial.printf("data[1]: %d\n", OutputintArray[1]);
    Serial.printf("data[2]: %d\n", OutputintArray[2]);
    Serial.printf("data[3]: %d\n", OutputintArray[3]);

  return OutputintArray;
}

void SplitAndBuiltInteger(){
    int currPeriod = millis()/5000;

  if (currPeriod != lastPeriod)
  {
    lastPeriod = currPeriod;





  int RandNum = 268435451;
  
  int Outputint2;

  int* buffer = IntegerSplit_4ByteMax(RandNum);

  Serial.printf("buffer: %d\n", buffer);
  Serial.print("buffer: "); 
  for (int i = 0; i<4;i++)
    {Serial.println(buffer[i]);
   }

   //Outputint2 = IntegerReconstruct_4ByteMax(buffer);
   
    Serial.printf("Outputint2: %d\n", Outputint2);
    //Serial.print("Outputint2: "); Serial.println(Outputint2);


  }
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
//using namespace std;

void DecodeIncomingMsg(){
  uint8_t command;
  uint32_t commandData[60];
  bool commandFound = false;

      Serial.print("DecodedMsg: ");
      for(int i = 0; i < IncomingMSG_Lenght;  i++ ){
        if((IncomingMsg_int[i] & 0x80) == 0x80){ //0x80=0b 1000 0000=128
          Serial.print("[");Serial.print(IncomingMsg_int[i] & 0x7F);Serial.print("]");
        }
        else{
          Serial.print("-");Serial.print(IncomingMsg_int[i]/* & 0x7F*/);Serial.print("-");
        }
      }
      Serial.println("");

      uint8_t commandData_Count = 0;
      for(int i = 0; i < IncomingMSG_Lenght;  i++ ){
        if (((IncomingMsg_int[i] & 0x80) == 0x80) && commandFound == false) { //s "|"
          commandFound = true;
          command = IncomingMsg_int[i] & 0x7F; //0111 1111
          commandData_Count = 0;
          //Serial.print("/");Serial.print("/CommandIS:");Serial.print(command);Serial.print("/");
        }else if(((IncomingMsg_int[i] & 0x80) == 0x80) && commandFound == true || i== IncomingMSG_Lenght) {//end of last command and new command found
          
          commandFound = false;
          //Serial.println("");
          //Serial.print("commandData: ");
          /*for (int i = 0; i < commandData_Count ;i++){
            Serial.print(commandData[i]);Serial.print("|");
          }*/
          //;Serial.println("");
          RunCommand(command, commandData_Count, commandData);
          commandData_Count = 0;
          i--;
        }else if(commandFound == true){
          commandData[commandData_Count] = IncomingMsg_int[i];
          commandData_Count++;
        }


      }
      RunCommand(command, commandData_Count, commandData);
}


void RFM_Recive_msg(){
  RFM_Recive_msg_DispFlag = true;
  //string s1("Geeks");


  if (radio.receiveDone())
    {
      /*Serial.print("Got:[");
      Serial.print(++packetCount);
      Serial.print(']');
      Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
      if (promiscuousMode)
      {
        Serial.print("to [");Serial.print(radio.TARGETID, DEC);Serial.print("] ");
      }*/
      packetCount++;
      for(int b = 0; b < sizeof(IncomingMsg_char);  ++b )
        IncomingMsg_char[b] = (char)0;

      //memset(IncomingMsg, 0, sizeof(IncomingMsg)); 
      Serial.println("---NEW MESSAGE---");
      Serial.print("radio.DATALEN: ");Serial.println(radio.DATALEN);
      //Serial.print("IncomingMSG: ");
      IncomingMSG_Lenght = radio.DATALEN;
      for (byte i = 0; i < radio.DATALEN; i++){
        //Serial.print((char)radio.DATA[i]);
        IncomingMsg_char[i] = (char)radio.DATA[i];
        IncomingMsg_int[i] = radio.DATA[i];
      }
      //DecodeIncomingMsg();


      //Conver incoming msg to int
      //IncomingMsg_INT = atoi(IncomingMsg);
      //int someInt = IncomingMsg - '0';



      /*
      if (prevMsg != IncomingMsg_INT - 1 && prevMsg !=0){
        MissedMsg++;
      }
      prevMsg = IncomingMsg_INT;*/


      Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
      //Serial.print("   Int:");Serial.print(IncomingMsg_INT);
      CurrentRSSI = radio.RSSI;
      if (radio.ACKRequested())
      {
        byte theNodeID = radio.SENDERID;
        radio.sendACK();
        Serial.print(" - ACK sent.");

        // When a node requests an ACK, respond to the ACK
        // and also send a packet requesting an ACK (every 3rd one only)
        // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
        if (ackCount++%3 == 0)
        {
          Serial.print(" Pinging node ");
          Serial.print(theNodeID);
          Serial.print(" - ACK...");
          delay(3); //need this when sending right after reception .. ?

          // sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime)
          if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
          {
            Serial.print("ok!");
          }
          else
          {
            Serial.print("nothing");
          }
        }
      }
      Serial.println();
      Blink(onboardLED, 3);
    }
}

void RunCommand(uint8_t command, uint8_t commandData_Count, uint32_t commandData[60]){
    uint32_t Outputint;
    //Serial.printf("Command: %s, length: %d\n", StalkerCommandList[command],commandData_Count);
    //Serial.print("Command: "); Serial.println(StalkerCommandList[command]);

    //Serial.print("CommandData: "); 
    //for (int i = 0; i< commandData_Count;i++){
    //  Serial.print("-");Serial.print(commandData[i]);Serial.print("-");
    //}
    //Serial.println("");
  switch (command) {
    case 0:
        Outputint = IntegerReconstruct_4ByteMax(commandData, commandData_Count);
        Serial.printf("Command: %s \tlength: %d \tdata: %d\n", StalkerCommandList[command],commandData_Count,Outputint);
        //Serial.printf("Outputint: %d\n", Outputint);
        //Serial.print("Outputint3: "); Serial.println(Outputint3);
    break;

    case 1:
        Outputint = IntegerReconstruct_4ByteMax(commandData, commandData_Count);
        Serial.printf("Command: %s \tlength: %d \tdata: %d\n", StalkerCommandList[command],commandData_Count,Outputint);
    break;

    case 2:
        Outputint = IntegerReconstruct_4ByteMax(commandData, commandData_Count);
        Serial.printf("Command: %s \tlength: %d \tdata: %d\n", StalkerCommandList[command],commandData_Count,Outputint);
    break;

    case 3:
        Outputint = IntegerReconstruct_4ByteMax(commandData, commandData_Count);
        Serial.printf("Command: %s \tlength: %d \tdata: %d\n", StalkerCommandList[command],commandData_Count,Outputint);
    break;

    case 4:
        Outputint = IntegerReconstruct_4ByteMax(commandData, commandData_Count);
        Serial.printf("Command: %s \tlength: %d \tdata: %d\n", StalkerCommandList[command],commandData_Count,Outputint);
    break;

    case 5:
        Serial.printf("Command: %s \tlength: %d \tdata: ", StalkerCommandList[command],commandData_Count);
        for(int i = 0; i<commandData_Count; i++){
          Serial.printf("%c", commandData[i]);
        }
        Serial.printf("\n");
    break;

    default:
      int alma = 1;
    break;
  }



    return;
}


void RFM_Send_msg(){
  RFM_Send_msg_DispFlag = true;
  int currPeriod = millis()/TRANSMITPERIOD;
  //Serial.println(currPeriod);
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
      
    //Serial.print("radio.send() - ");Serial.print(millis());Serial.println(" ms");
    //radio.send(GATEWAYID, payload, sendSize,false);
    //Serial.print("radio.send() - ");Serial.print(millis());Serial.println(" ms");

        radio.send(GATEWAYID, payload, sendSize,false);
    /*if (radio.sendWithRetry(GATEWAYID, payload, sendSize))
    {
     Serial.print(" ok!");
    }   
    else
    {
     Serial.print(" nothing...");
    }*/

    //radio.send(GATEWAYID, payload, sendSize,false);
    sendSize = (sendSize + 1) % 7;
    Serial.println();
    Blink(onboardLED, 3);
  }
}


/*
void DrawIcon(){


  if (currentMillis - previousMillis_LoadingIcon >= 100) {
    // save the last time you blinked the LED
    previousMillis_LoadingIcon = currentMillis;
    IconState++;
    if(IconState == 4){
      IconState = 0;
    }
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
    //Serial.print("IconState: ");Serial.println(IconState);
  }



  switch (IconState){
    case 0:
      u8x8.setInverseFont(0);
      u8x8.draw2x2String(14, 0, "|");
      break;
    case 1:
      u8x8.setInverseFont(0);
      u8x8.draw2x2String(14, 0, "\x5c");
      //u8x8.setCursor(14,0);
      //u8x8.write(93);
      break;
    case 2:
      u8x8.setInverseFont(0);
      u8x8.draw2x2String(14, 0, "-");
      break;
    case 3:
      u8x8.setInverseFont(0);
      u8x8.draw2x2String(14, 0, "/");
      break;  
    }
    

}
*/


void AliveAnimation(){


  if (currentMillis - previousMillis_LoadingIcon >= 80) {
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
  if (RFM_Recive_msg_DispFlag == true){
    u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
    u8g2.setCursor(0,7); 
    u8g2.print("RSSI: ");u8g2.print(CurrentRSSI);
    u8g2.setCursor(0,14); 
    u8g2.print("Got:[");u8g2.print(packetCount);u8g2.print(']');u8g2.print(IncomingMsg_char);
  }
  if (RFM_Send_msg_DispFlag == true){
    u8g2.setFont(u8g2_font_5x8_tf); //Width - 4; Height - 6
    u8g2.setCursor(0,21); 
    u8g2.print("Sent: ");u8g2.print(SentMsg); 

  }

  u8g2.sendBuffer();          // transfer internal memory to the display
}








