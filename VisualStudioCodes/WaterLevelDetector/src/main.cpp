//----Includes----Start
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include <ESP8266WiFi.h>
#include <time.h>
#include <ESP8266HTTPClient.h>
#include <NewPing.h>
#include <EEPROM.h>
#include <DHT.h>
//----Includes----End

//----Declarations----Start
//const int trigPin = 14;
//const int echoPin = 12;
const int SerialID = 115200;
const char Def_ssid[32] = "KisMegaNet-Kapu";
const char Def_password[32] = "BicskeCity";
const char* host = "api.thingspeak.com";//Google:"64.233.187.99";//"api.thingspeak.com"; //api.thingspeak.com - 184.106.153.149
//byte host[] = { 64, 233, 187, 99 }; // Google
const int httpsPort = 80;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;


char CurrentButton;
long duration;
int ValidDistance;
int FileredValidDistance;
int SavedFileredValidDistance;
int DistanceBlock[5] = {0,0,0,0,0};
int DistanceBlockCounter = 0;
int CounterForValidEcho = 0;
int ProgramLevel = 0;
int ssid_i;
int password_i;
int lastProgramLevel;
int lastReparameterSelection;
int CheckWifiConnectionTimeoutCounter;
int CheckInternetConnectionTimeoutCounter;
int InternetSignalFlag = 0;
int MenuSelection = 1;
int ReparameterSelection = 1;
int num1;
int num2;
int num3;
int num4;
int numberFinal;
int NumTemp = 0;
int NumberInputLevel = 1;
int FullTankLiter;
int CurrentTankLiter;
int CurrentLvLPercent;
float h;
float t;
boolean NumberInputFlag = 0;
boolean NumberInputEnd = 0;
boolean InternetAnimationFlag = 0;
boolean WifiConnection = 0; //0 - Not connected to Wifi, 1 - Connected to wifi
boolean InternetConnection = 0; //0 - Not connected to Wifi, 1 - Connected to wifi
boolean CheckWifiConnectionTimeoutFlag = 0;
boolean CheckInternetConnectionTimeoutFlag = 0;
boolean ClearLCD_OneFlag;
boolean SaveReparameter;
boolean Button_Up_LastState = 1;
boolean Button_Ok_LastState = 1;
boolean Button_Down_LastState = 1;
boolean Button_Back_LastState = 1;

//EEPROM Varaiables
int FirstReparameterAfterFlash;
int MinTankEchodist;	
int MaxTankEchodist; 			
int DorAside;					
int Bside; 								
int Period_USTime;		
int EchoHysteresis;	
int NewEchoCounter;
int ExtendedByte;
boolean TankTypeCorS; //0 - Circle | 1-Squaer
char ssid[32];
char password[32];


//bool FirstRunAfterFlash;
//int FirstRun;
//----Declarations----End

//----Defines----Start
#define Period_InternetCheck 1000
#define Period_ShowTime 1000
#define Period_SendDataToInternet 30000


#define Period_US_LCD_Printout 1000 //500 LCD refres rate in ms
#define Period_ActiveInternetConnectionAnimation 750
#define Period_CheckWifiConnection 1000
#define Period_CheckWifiConnectionTimeout 10000
#define Period_CheckInternetConnectionTimeout 10000
#define trigPin 0
#define echoPin 15
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302)

#define Button_Up 14
#define Button_Ok 12
#define Button_Down 13
#define Button_Back 3
//EEPROM Defines
#define Def_EchoHysteresis 10 //cm
#define Def_NewEchoCounter 5 //30pcs
#define Def_Period_USTime 1000  //50 Echo sample time in ms
#define Def_MinTankEchodist 25
#define Def_MaxTankEchodist 150
#define Def_DorAside 140 //Diameter of tank or A value of sqear tank
#define Def_Bside 0 //B value of tank
#define Def_TankTypeCorS 0 //Circle or Squear Tank
#define OverWriteDefaultEEPROM 0 //This value should be 0 for final version!
//----Defines----End


//----TimeDeclarations----Start
long lastMillis_InternetConnection;
long lastMillis_USTime;
long lastMillis_US_LCD_Printout;
long lastMillis_ShowTime;
long lastMillis_ActiveInternetConnectionAnimation;
long lastMillis_CheckWifiConnection;
long lastMillis_CheckWifiConnectionTimeout;
long lastMillis_CheckWifiConnectionTimeoutCounter;
long lastMillis_CheckInternetConnectionTimeout;
long lastMillis_CheckInternetConnectionTimeoutCounter;
long lastMillis_ReadDHT;
long lastMillis_ShowDHT;
long lastMillis_SendDataToInternet;
//----TimeDeclarations----End

//----OtherDeclarations----Start
WiFiClient client;
NewPing sonar(trigPin, echoPin, MAX_DISTANCE);
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD Declaration
DHT dht(DHTPIN, DHTTYPE);
float tValue;// 

byte InternetSignal1[] = { //Wifi Signal Symbom
  B00100,
  B01110,
  B00100,
  B00100,
  B00100,
  B00100,
  B01110,
  B11111};
byte InternetSignal2[] = { //Wifi Signal Symbom
  B10101,
  B11111,
  B10101,
  B00100,
  B00100,
  B00100,
  B01110,
  B11111};
  byte WifiSignal[] = { //Wifi Signal Symbom
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B01110,
  B11111};
  byte NoWifiSignal[] = { //Wifi Signal Symbom
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111};
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
//----OtherDeclarations----End
//----OtherInfo----Start
/*
ESP8266 has 512 bytes of internal EEPROM
EEPROM sturct:
0. 				FirstReparameterAfterFlash	[1byte]											
1-2. 			Min Tank Echo dist 			[2byte] [1-400]cm
3-4. 			Max Tank Echo dist 			[2byte] [1-400]cm
5-6. 			Diameter or A of Tank 		[2byte] [1-6400]cm
7-8. 			B length of tank 			[2byte] [1-6400]cm
9-10. 			Period_USTime				[2 byte][1-65536]ms
11. 			EchoHysteresis		[1 byte][1-256]cm
12. 			NewEchoCounter	[1byte][1-256]pcs
13.				Def_TankTypeCorS    		[1byte]

100-131. 		SSID 						[200byte]
200-231. 		Password 					[200byte]
*/
//----OtherInfo----End

void setup() {
//----Setup Serial comm----start
Serial.begin(SerialID);
delay(1000);
Serial.println("");
Serial.print("Serial.begin:");Serial.println(SerialID);
//----Setup Serial comm----End

//----Other Init----start
EEPROM.begin(512);  //Initialize EEPROM
dht.begin();
//----Other Init----end

//----LCD init----Start
lcd.init();
lcd.backlight();
lcd.createChar(0, WifiSignal);
lcd.createChar(1, InternetSignal1);
lcd.createChar(2, InternetSignal2);
lcd.createChar(3, NoWifiSignal);
//----LCD init----End
//----Init_pinModes----Start
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(LED_BUILTIN, OUTPUT);
pinMode(Button_Up,INPUT_PULLUP);
pinMode(Button_Ok,INPUT_PULLUP);
pinMode(Button_Down,INPUT_PULLUP);
pinMode(Button_Back,INPUT_PULLUP);
//----Init_pinModes----End

//----Functions----Start
//ConnectToWifi();
//SetupClock();
//----Functions----End
}


void loop() {
//----Functions----Start
MainProgram();
programLevelTrack(); //DevelopmentTrack
//CheckInternetConnection();
//----Functions----End
delay(1);
}

void MainProgram(){
	long currentMillis = millis();
	
	

   switch (ProgramLevel) {
   	  case 0: //First run after flash? If yes go to 6. (reparameter)
   	  	if (OverWriteDefaultEEPROM == 1){
   	  		EEPROM.write(0, 1); 									//[1byte]				
   	  		EEPROM.write(1, highByte(Def_MinTankEchodist));			//[2byte] [1-400]cm
   	  		EEPROM.write(2, lowByte(Def_MinTankEchodist)); 			//[2byte] [1-400]cm
   	  		EEPROM.write(3, highByte(Def_MaxTankEchodist)); 		//[2byte] [1-400]cm
   	  		EEPROM.write(4, lowByte(Def_MaxTankEchodist)); 			//[2byte] [1-400]cm
   	  		EEPROM.write(5, highByte(Def_DorAside)); 				//[2byte] [1-6400]cm
   	  		EEPROM.write(6, lowByte(Def_DorAside)); 				//[2byte] [1-6400]cm
   	  		EEPROM.write(7, highByte(Def_Bside)); 					//[2byte] [1-6400]cm
   	  		EEPROM.write(8, lowByte(Def_Bside)); 					//[2byte] [1-6400]cm
   	  		EEPROM.write(9, highByte(Def_Period_USTime));			//[2 byte][1-65536]ms
   	  		EEPROM.write(10, lowByte(Def_Period_USTime)); 			//[2 byte][1-65536]ms

   	  		EEPROM.write(11, Def_EchoHysteresis); 			//[1 byte][1-256]cm
   	  		EEPROM.write(12, Def_NewEchoCounter);		//[1byte][1-256]pcs
   	  		EEPROM.write(13, Def_TankTypeCorS);		//[1byte][1-256]pcs
			
   	  		ssid_i = 100;
			for(int i=0; i< sizeof(Def_ssid); i++){
				EEPROM.write(ssid_i, Def_ssid[i]);
				ssid_i++;
				Serial.print(Def_ssid[i]);
			}

			password_i = 200;
			for(int i=0; i< sizeof(Def_password); i++){
				EEPROM.write(password_i, Def_password[i]);
				password_i++;
				Serial.print(Def_password[i]);
			}

   	  		EEPROM.commit();
   	  		

   	  	}

   	FirstReparameterAfterFlash		= EEPROM.read(0); 	
		MinTankEchodist					      = (EEPROM.read(1) << 8) + EEPROM.read(2); //reconstruct the integer
		MaxTankEchodist 				      = (EEPROM.read(3) << 8) + EEPROM.read(4); 			
		DorAside 						          = (EEPROM.read(5) << 8) + EEPROM.read(6); 						
		Bside 							          = (EEPROM.read(7) << 8) + EEPROM.read(8); 						
		Period_USTime				         	= (EEPROM.read(9) << 8) + EEPROM.read(10);
		EchoHysteresis 			          = EEPROM.read(11); 		
		NewEchoCounter 		            = EEPROM.read(12);
		TankTypeCorS 					        = EEPROM.read(13);

		
		ssid_i = 100;
		for(int i=0; i<32 ;i++){
			ssid[i] = EEPROM.read(ssid_i);
			ssid_i++;
		}
		
		password_i = 200;
		for(int i=0; i<32 ;i++){
			password[i] = EEPROM.read(password_i);
			password_i++;
		}		

		
		Serial.print("EEPROM_0: FirstReparameterAfterFlash: \t"); Serial.println(FirstReparameterAfterFlash);
		Serial.print("EEPROM_1-2: MinTankEchodist: \t"); Serial.println(MinTankEchodist);
		Serial.print("EEPROM_3-4: MaxTankEchodist: \t"); Serial.println(MaxTankEchodist);
		Serial.print("EEPROM_5-6: Def_DorAside: \t"); Serial.println(Def_DorAside);
		Serial.print("EEPROM_7-8: Def_Bside: \t"); Serial.println(Def_Bside);
		Serial.print("EEPROM_9-10: Period_USTime: \t"); Serial.println(Period_USTime);
		Serial.print("EEPROM_11: EchoHysteresis: \t"); Serial.println(EchoHysteresis);
		Serial.print("EEPROM_12: NewEchoCounter: \t"); Serial.println(NewEchoCounter);
		Serial.print("EEPROM_13: TankTypeCorS: \t"); Serial.println(TankTypeCorS);

		Serial.print("EEPROM_100-131: ssid: \t"); Serial.println(ssid);
		Serial.print("EEPROM_200-231: password: \t"); Serial.println(password);


    if(TankTypeCorS == 0){
      FullTankLiter = ((DorAside/2)*(DorAside/2)*3.1415*(MaxTankEchodist-MinTankEchodist))/1000;
      //lcd.setCursor(0, 1);lcd.print(FullTankLiter);lcd.print(" Liter          ");
    }else if(TankTypeCorS ==1){
      FullTankLiter = (DorAside*Bside*(MaxTankEchodist-MinTankEchodist))/1000;
      //lcd.setCursor(0, 1);lcd.print(FullTankLiter);lcd.print(" Liter          ");
    }
    Serial.print("FullTankLiter: \t"); Serial.println(FullTankLiter);



   	  	if(FirstReparameterAfterFlash == 1){
   	  		//Move to Reparameter section.
   	  	}

   	  	ProgramLevel = 1;
        break;

      case 1: //Connect to wifi with default data
      	ConnectToWifi();
      	if (WifiConnection == 0){ // if not succesfull
      		ProgramLevel =21; //<<----------------------------------------------------------------------------------ez 11
      	}else if(WifiConnection == 1){ // if succesfull
      		ProgramLevel =13;
      	}
        break;

	      case 11: //Scan Wifi Network
	      	ShowTime();
	        
	        break; 

	      case 12: //Check Wifi Connection
	      	CheckWifiConnection();

	      	//-------------10 sec Timeout-----------------
	      	if (CheckWifiConnectionTimeoutFlag == 0){
	      		lastMillis_CheckWifiConnectionTimeout = currentMillis;
	      		CheckWifiConnectionTimeoutCounter =Period_CheckWifiConnectionTimeout/1000;
	      		CheckWifiConnectionTimeoutFlag = 1;
	      	}
	      	if (currentMillis - lastMillis_CheckWifiConnectionTimeoutCounter > 1000){
	      		Serial.print("Checking Wifi: \t"); Serial.println(CheckWifiConnectionTimeoutCounter);

    			lcd.setCursor(0, 0); lcd.print("Checking");
  				lcd.setCursor(0, 1); lcd.print("Wifi...");
  				lcd.setCursor(0, 1); lcd.print(CheckWifiConnectionTimeoutCounter);
  				CheckWifiConnectionTimeoutCounter--;
  				lastMillis_CheckWifiConnectionTimeoutCounter = currentMillis;
  			}	      	
	      	if (currentMillis - lastMillis_CheckWifiConnectionTimeout > Period_CheckWifiConnectionTimeout){
    			//WifiConnection Timeouted...
    			Serial.println("No Wifi Connection. Wait 3 sec.");
    			ClearLCD_ExceptLastSymbol();
    			lcd.setCursor(0, 0); lcd.print("No Wifi");
  				lcd.setCursor(0, 1); lcd.print("Connection...");
  				delay(3000);
  				ClearLCD_ExceptLastSymbol();
    			ProgramLevel  = 21;
    			CheckWifiConnectionTimeoutFlag = 0;
  			}
  			//---------------10 sec Timeout---------------
  			if (WifiConnection == 1){
  				Serial.println("Wifi Connection is OK. Wait 3 sec.");
  				ClearLCD_ExceptLastSymbol();
  				lcd.setCursor(0, 0); lcd.print("Wifi");
  				lcd.setCursor(0, 1); lcd.print("Connection is OK");
  				delay(3000);
  				ClearLCD_ExceptLastSymbol();
  				ProgramLevel  = 13;
  				CheckWifiConnectionTimeoutFlag = 0;
  			}
	        
	        break; 

	      case 13: //Check Internet Connection
	      	CheckInternetConnection();

	      	//-------------10 sec Timeout-----------------
	      	if (CheckInternetConnectionTimeoutFlag == 0){
	      		lastMillis_CheckInternetConnectionTimeout = currentMillis;
	      		CheckInternetConnectionTimeoutCounter = Period_CheckInternetConnectionTimeout/1000;
	      		CheckInternetConnectionTimeoutFlag = 1;
	      	}
	      	if (currentMillis - lastMillis_CheckInternetConnectionTimeoutCounter > 1000){
	      		Serial.print("Checking Internet Connection: \t"); Serial.println(CheckWifiConnectionTimeoutCounter);

    			lcd.setCursor(0, 0); lcd.print("Checking");
  				lcd.setCursor(0, 1); lcd.print("Internet...");
  				lcd.setCursor(0, 1); lcd.print(CheckInternetConnectionTimeoutCounter);
  				CheckInternetConnectionTimeoutCounter--;
  			}	
	      	if (currentMillis - lastMillis_CheckInternetConnectionTimeout > Period_CheckWifiConnectionTimeout){
    			//InternetConnection Timeouted...
    			Serial.println("No Internet Connection. Wait 3 sec.");
    			ClearLCD_ExceptLastSymbol();
    			lcd.setCursor(0, 0); lcd.print("No Internet");
  				lcd.setCursor(0, 1); lcd.print("Connection...");
  				delay(3000);
  				ClearLCD_ExceptLastSymbol();    			
    			ProgramLevel  = 22;
    			CheckInternetConnectionTimeoutFlag = 0;
  			}
  			//---------------10 sec Timeout---------------
  			if (WifiConnection == 1){
    			Serial.println("Internet Connection is ok. Wait 3 sec.");
    			ClearLCD_ExceptLastSymbol();
    			lcd.setCursor(0, 0); lcd.print("Internet");
  				lcd.setCursor(0, 1); lcd.print("Connection is ok");
  				delay(3000); 
  				ClearLCD_ExceptLastSymbol(); 				
  				ProgramLevel  = 14;
  				CheckInternetConnectionTimeoutFlag = 0;
  			}	        
	        break;

	      case 14: //Get time from SMTP
	      SetupClock();
	      ProgramLevel = 2;
        
        	break;

      case 2:	// Main Measurement program     (Tank lvl: 100% W)
            //                                (2000/2000L 23.1C)
      			// +Send Data to internet
      			// +Check Wifi -->If fail goto -21
      			// +Check Internet -->If fail goto -21 (Make sure not the wifi is the problem)
      	
      	//ClearLCD_One();
      	if(ClearLCD_OneFlag==0){
			ClearLCD_ExceptLastSymbol();
			//lcd.setCursor(0, 0); lcd.print("Main MP (2)");
			//Serial.println("Main MP (2)");
			ClearLCD_OneFlag = 1;
			NumberInputFlag = 0;
		  }

        UltraSonicMeasurement();
        ReadDHT();
        TankLvlDetectorView();
        SendDataToInternet();

      	CheckWifiConnection();
      	CheckInternetConnection();
      	ActiveInternetConnectionAnimation();
		    //ShowTime();

		if(WifiConnection == 0){
			ProgramLevel = 21;
			ClearLCD_OneFlag = 0;
		}
		if(InternetConnection == 0){ //Check Wifi Connection first
			CheckWifiConnection();
			if(WifiConnection == 0){
				ProgramLevel = 21;
				ClearLCD_OneFlag = 0;
			}
			ProgramLevel = 22;
			ClearLCD_OneFlag = 0;
		}
		if('b' == ButtonHandling()){ //Pressing Back button
			ProgramLevel = 3;
			ClearLCD_OneFlag = 0;
		}


        
        break;

	      case 21: 	// Main Measurement program
      				// +Try to reconnect to wifi --> if success go to 12
	      	//Write out
	        if (CheckWifiConnectionTimeoutFlag == 0){
	        	CheckWifiConnectionTimeoutFlag =1;
	        	ClearLCD_ExceptLastSymbol();
	        	lcd.setCursor(0, 0); lcd.print("Wifi");
  				  lcd.setCursor(0, 1); lcd.print("Connection lost!");
  				  delay(3000);
  				  ClearLCD_ExceptLastSymbol();
	        }
	        ClearLCD_One();

      		UltraSonicMeasurement();
          ReadDHT();
          TankLvlDetectorView();
      		CheckWifiConnection();
      		CheckInternetConnection();
      		ActiveInternetConnectionAnimation();

		if('b' == ButtonHandling()){ //Pressing Back button
			ProgramLevel = 3;
			ClearLCD_OneFlag = 0;
		}

			

			if(WifiConnection == 1){
			CheckWifiConnectionTimeoutFlag == 0;				
			ProgramLevel = 12;
			ClearLCD_OneFlag = 0;
		    }      
	        
	        break;	// Main Measurement program
      				// +Try to ping --> if success go to 13

	      case 22:
	      	if (CheckInternetConnectionTimeoutFlag == 0){
	        	CheckInternetConnectionTimeoutFlag = 1;
	        	ClearLCD_ExceptLastSymbol();
	        	lcd.setCursor(0, 0); lcd.print("Wifi");
  				  lcd.setCursor(0, 1); lcd.print("Connection lost!");
  				  delay(3000);
  				  ClearLCD_ExceptLastSymbol();
	        }

	        ClearLCD_One();
      		UltraSonicMeasurement();
          ReadDHT();
          TankLvlDetectorView();

      		CheckWifiConnection();
      		CheckInternetConnection();
      		ActiveInternetConnectionAnimation();


      	if('b' == ButtonHandling()){ //Pressing Back button
			ProgramLevel = 3;
			ClearLCD_OneFlag = 0;
		}


			if(InternetConnection == 0 && WifiConnection == 0){
			CheckInternetConnectionTimeoutFlag = 0;
			ProgramLevel = 21;
			ClearLCD_OneFlag = 0;
			}

			if(InternetConnection == 1){
			CheckInternetConnectionTimeoutFlag = 0;
			ProgramLevel = 13;
			ClearLCD_OneFlag = 0;
		    }  
	        
	        break;

      case 3:	// Program Selection screen
      			// 1. Scan Wifi Network  --> ok -->goto 11
      			// 2. Raw Echo check --> ok -->goto 4
      			// 3. Show Date & Time --> ok -->goto 5
      			// 4. Reparameter --> ok -->goto 6
      			// 5. Echo calc parameter
      			// 6. DHT???
      		//ShowTime();	
      		ClearLCD_One();
      		
      		lcd.setCursor(0, 0);lcd.print("Menu:");
      		lcd.setCursor(13, 0);lcd.print(MenuSelection);lcd.print("/5");

      		CurrentButton = ButtonHandling();
      		if('b' == CurrentButton){ //Pressing Back button
			if(WifiConnection == 0){
				ProgramLevel = 21;
			}else if(InternetConnection == 0){
				ProgramLevel = 22;
			}else{
				ProgramLevel = 2;
			}

			ClearLCD_OneFlag = 0;
			}
			if('u' == CurrentButton){ //Pressing Up button
				if (MenuSelection == 5){
					MenuSelection = 1;
				}else{
					MenuSelection++;
				}
				Serial.print("MenuSelection: "); Serial.println(MenuSelection);
			
			}
			if('d' == CurrentButton){ //Pressing Down button
				if (MenuSelection == 1){
					MenuSelection = 5;
				}else{
					MenuSelection--;
				}
				Serial.print("MenuSelection: "); Serial.println(MenuSelection);
			}
			switch (MenuSelection){
				case 1:
					lcd.setCursor(0, 1);lcd.print("Scan WifiNetwork");
				break;
				case 2:
					lcd.setCursor(0, 1);lcd.print("Raw Echo Check  ");
					if('o' == CurrentButton){ //Pressing OK button
						ProgramLevel = 4;
						ClearLCD_OneFlag = 0;
					}
				break;
				case 3:
					lcd.setCursor(0, 1);lcd.print("Show Date & Time");
					if('o' == CurrentButton){ //Pressing OK button
						ProgramLevel = 5;
						ClearLCD_OneFlag = 0;
					}

				break;
				case 4:
					lcd.setCursor(0, 1);lcd.print("Reparameter     ");
					if('o' == CurrentButton){ //Pressing OK button
						ProgramLevel = 6;
						ClearLCD_OneFlag = 0;
					}
				break;
				case 5:
					lcd.setCursor(0, 1);lcd.print("Temp & Humidity");
					if('o' == CurrentButton){ //Pressing OK button
						ProgramLevel = 7;
						ClearLCD_OneFlag = 0;
					}
				break;

			}
        
        break;

      case 4:	//Raw Echo check --> B --> goto2
      		ClearLCD_One();
        	UltraSonicMeasurement();
          RawEchoShow();
            CurrentButton = ButtonHandling();
      		if('b' == CurrentButton){ //Pressing Back button
				ProgramLevel = 2;
				ClearLCD_OneFlag = 0;
			}      
        
        break;

      case 5:	//Show Date & Time --> B --> goto2
      		ClearLCD_One();
        	ShowTime();
             CurrentButton = ButtonHandling();
      		if('b' == CurrentButton){ //Pressing Back button
				ProgramLevel = 2;
				ClearLCD_OneFlag = 0;
			}
        break;

      case 6: 	//Reparameter --> B --> goto2
      			//1. Empty tank echo dist 					0-511 --> A
      			//2. Max tank echo dist 					0-511 --> A
      			//3. Tank Shape? circle (d) or squear (a+b) 0-1   --> A
      			//4. Tank Diameter d or a 					0-511 --> A
      			//5. Tank b 								0-511 --> A
      			//6. Save 				 					A or B (if B go to 2)
      			//7. Calculated max water ammount (wait 5 sec then go to 2)
      			//If first run after flash --> set FirstRun bit
      		ClearLCD_One();
      		if(NumberInputFlag == 0){
      			lcd.setCursor(13, 0);lcd.print(ReparameterSelection);lcd.print("/9");
      	    }

				//ReparameterSelection
			switch (ReparameterSelection){
				case 1:
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("Min T. Echo  ");
						Serial.print("MinTankEchodist: "); Serial.println(MinTankEchodist);
						if(NumberInputLevel == 1){
							num1 = MinTankEchodist/100; 
	      					num2 = (MinTankEchodist-num1*100)/10; 
	      					num3 = MinTankEchodist-num1*100-num2*10; 
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
	      					Serial.print("num2: "); Serial.println(num2);
	      					Serial.print("num3: "); Serial.println(num3);
	      					lcd.setCursor(3, 1);lcd.print("cm");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						MinTankEchodist = numberFinal;
						NumberInputEnd = 0;
						Serial.print("MinTankEchodist: "); Serial.println(MinTankEchodist);
						ReparameterSelection++;
					}

					if('b' == CurrentButton){ //Pressing Back button
					ProgramLevel = 2;
					ClearLCD_OneFlag = 0;
					lcd.noBlink();
					lcd.clear();
					NumberInputLevel = 1;
					}

				break;
				case 2:
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("Max T. Echo  ");
						//num1 = MaxTankEchodist/100; 
      					//num2 = (MaxTankEchodist-num1*100)/10; 
      					//num3 = MaxTankEchodist-num1*100-num2*10;
      					if(NumberInputLevel == 1){
      						num1 = MaxTankEchodist/100; 
      						num2 = (MaxTankEchodist-num1*100)/10; 
      						num3 = MaxTankEchodist-num1*100-num2*10;
      						NumTemp=num1;
      						Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("cm");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						MaxTankEchodist = numberFinal;
						NumberInputEnd = 0;
						Serial.print("MaxTankEchodist: "); Serial.println(MaxTankEchodist);
						ReparameterSelection++;
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					}					
				break;
				case 3:
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("Tank Shape?  ");
						lcd.setCursor(0, 1);lcd.print("Circle or Square");
						lcd.setCursor(0, 1);
						TankTypeCorS = 0;
						lcd.blink();
						NumberInputFlag = 1;
					}
					CurrentButton = ButtonHandling();
					if('u' == CurrentButton){ //Pressing Up button
						TankTypeCorS = 1;
						lcd.setCursor(10, 1);
					}
					if('d' == CurrentButton){ //Pressing Down button
						TankTypeCorS = 0;
						
						lcd.setCursor(0, 1);
					}
					if('o' == CurrentButton){ //Pressing OK button
						ReparameterSelection++;
						NumberInputFlag = 0;
						lcd.setCursor(0, 1);lcd.print("                ");
						Serial.print("TankTypeCorS: "); Serial.println(TankTypeCorS);
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					lcd.setCursor(0, 1);lcd.print("                ");
					}

				break;
				case 4:
					//lcd.setCursor(0, 0);lcd.print("Tank D or A  ");
					if(NumberInputFlag == 0 && TankTypeCorS == 0){
						lcd.setCursor(0, 0);lcd.print("Tank diameter");
						
						if(NumberInputLevel == 1){
							num1 = DorAside/100; 
	      					num2 = (DorAside-num1*100)/10; 
	      					num3 = DorAside-num1*100-num2*10;
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("cm");
      					}
					}else if(NumberInputFlag == 0 && TankTypeCorS == 1){
						lcd.setCursor(0, 0);lcd.print("Tank A Side: ");
						if(NumberInputLevel == 1){
							num1 = DorAside/100; 
	      					num2 = (DorAside-num1*100)/10; 
	      					num3 = DorAside-num1*100-num2*10;
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("cm");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						DorAside = numberFinal;
						NumberInputEnd = 0;
						Serial.print("DorAside: "); Serial.println(DorAside);
						if (TankTypeCorS == 0){
							ReparameterSelection=6;
						}else{
							ReparameterSelection++;
						}
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					}
				break;
				case 5:
					//lcd.setCursor(0, 0);lcd.print("Tank B side  ");
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("Tank B Side: ");
						if(NumberInputLevel == 1){
							num1 = Bside/100; 
	      					num2 = (Bside-num1*100)/10; 
	      					num3 = Bside-num1*100-num2*10;
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("cm");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						Bside = numberFinal;
						NumberInputEnd = 0;
						Serial.print("Bside: "); Serial.println(Bside);
						ReparameterSelection++;
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					}				
				break;
				case 6://--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("Period_USTime");
						if(NumberInputLevel == 1){
							num1 = Period_USTime/100; 
	      					num2 = (Period_USTime-num1*100)/10; 
	      					num3 = Period_USTime-num1*100-num2*10;
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("ms");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						Period_USTime = numberFinal;
						NumberInputEnd = 0;
						Serial.print("Period_USTime: "); Serial.println(Period_USTime);
						ReparameterSelection++;
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					}
				break;
				case 7://--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("EchoHysteresi");
						if(NumberInputLevel == 1){
							num1 = EchoHysteresis/100; 
	      					num2 = (EchoHysteresis-num1*100)/10; 
	      					num3 = EchoHysteresis-num1*100-num2*10;
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("cm");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						EchoHysteresis = numberFinal;
						NumberInputEnd = 0;
						Serial.print("EchoHysteresis: "); Serial.println(EchoHysteresis);
						ReparameterSelection++;
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					}
				break;					
				case 8://--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("NewEchoCount ");
						if(NumberInputLevel == 1){
							num1 = NewEchoCounter/100; 
	      					num2 = (NewEchoCounter-num1*100)/10; 
	      					num3 = NewEchoCounter-num1*100-num2*10;
	      					NumTemp=num1;
	      					Serial.print("num1: "); Serial.println(num1);
      						Serial.print("num2: "); Serial.println(num2);
      						Serial.print("num3: "); Serial.println(num3);
      						lcd.setCursor(3, 1);lcd.print("pcs");
      					}
					}
					NumberInput_3digit();
					if(NumberInputEnd == 1){
						NewEchoCounter = numberFinal;
						NumberInputEnd = 0;
						Serial.print("NewEchoCounter: "); Serial.println(NewEchoCounter);
						ReparameterSelection++;
					}
					if('b' == CurrentButton){ //Pressing Back button
					ReparameterSelection--;NumberInputFlag = 0; NumberInputLevel = 1;
					}
				break;					
				case 9:
					//lcd.setCursor(0, 0);lcd.print("Save?        ");
					if(NumberInputFlag == 0){
						lcd.setCursor(0, 0);lcd.print("Save?        ");
						lcd.setCursor(0, 1);lcd.print("Yes or No");
						lcd.setCursor(0, 1); SaveReparameter = 1; //Yes
						lcd.blink();
						NumberInputFlag = 1;
					}
					CurrentButton = ButtonHandling();
					if('u' == CurrentButton){ //Pressing Up button
						lcd.setCursor(7, 1);
						SaveReparameter = 0; //No
					}
					if('d' == CurrentButton){ //Pressing Down button
						lcd.setCursor(0, 1);
						SaveReparameter = 1; //Yes
					}
					if('o' == CurrentButton){ //Pressing OK button
						ReparameterSelection=0;
						NumberInputFlag = 0;
						lcd.noBlink();

					}
					if('b' == CurrentButton){ //Pressing Back button
						ReparameterSelection=0;
						NumberInputFlag = 0;
						lcd.noBlink();
					}
				break;	
				case 0:
					//lcd.setCursor(0, 0);lcd.print("Max liter:   ");
					if (SaveReparameter == 0){ //No){
						FirstReparameterAfterFlash		= EEPROM.read(0); 	
						MinTankEchodist					= (EEPROM.read(1) << 8) + EEPROM.read(2); //reconstruct the integer
						MaxTankEchodist 				= (EEPROM.read(3) << 8) + EEPROM.read(4); 			
						DorAside 						= (EEPROM.read(5) << 8) + EEPROM.read(6); 						
						Bside 							= (EEPROM.read(7) << 8) + EEPROM.read(8); 						
						Period_USTime					= (EEPROM.read(9) << 8) + EEPROM.read(10);
						EchoHysteresis 			= EEPROM.read(11); 		
						NewEchoCounter 		= EEPROM.read(12);
						TankTypeCorS 					= EEPROM.read(13);
						lcd.setCursor(0, 0);lcd.print("Parameters   ");
						lcd.setCursor(0, 1);lcd.print("not saved!      ");
						delay(2000);
					}else if (SaveReparameter == 1){
						EEPROM.write(1, highByte(MinTankEchodist));			//[2byte] [1-400]cm
						EEPROM.write(2, lowByte(MinTankEchodist)); 			//[2byte] [1-400]cm
						EEPROM.write(3, highByte(MaxTankEchodist)); 		//[2byte] [1-400]cm
						EEPROM.write(4, lowByte(MaxTankEchodist)); 			//[2byte] [1-400]cm
						EEPROM.write(5, highByte(DorAside)); 				//[2byte] [1-6400]cm
						EEPROM.write(6, lowByte(DorAside)); 				//[2byte] [1-6400]cm
						EEPROM.write(7, highByte(Bside)); 					//[2byte] [1-6400]cm
						EEPROM.write(8, lowByte(Bside)); 					//[2byte] [1-6400]cm
						EEPROM.write(9, highByte(Period_USTime));			//[2 byte][1-65536]ms
						EEPROM.write(10, lowByte(Period_USTime)); 			//[2 byte][1-65536]ms
						EEPROM.write(11, EchoHysteresis); 				//[1 byte][1-256]cm
						EEPROM.write(12, NewEchoCounter);		//[1byte][1-256]pcs
						EEPROM.write(13, TankTypeCorS);						//[1byte][1-256]pcs
						EEPROM.commit();
						lcd.setCursor(0, 0);lcd.print("Parameters   ");
						lcd.setCursor(0, 1);lcd.print("Saved!          ");
						delay(2000);
					}
					Serial.print("EEPROM_0: FirstReparameterAfterFlash: \t"); Serial.println(FirstReparameterAfterFlash);
					Serial.print("EEPROM_1-2: MinTankEchodist: \t"); Serial.println(MinTankEchodist);
					Serial.print("EEPROM_3-4: MaxTankEchodist: \t"); Serial.println(MaxTankEchodist);
					Serial.print("EEPROM_5-6: Def_DorAside: \t"); Serial.println(Def_DorAside);
					Serial.print("EEPROM_7-8: Def_Bside: \t"); Serial.println(Def_Bside);
					Serial.print("EEPROM_9-10: Period_USTime: \t"); Serial.println(Period_USTime);
					Serial.print("EEPROM_11: EchoHysteresis: \t"); Serial.println(EchoHysteresis);
					Serial.print("EEPROM_12: NewEchoCounter: \t"); Serial.println(NewEchoCounter);
					Serial.print("EEPROM_13: TankTypeCorS: \t"); Serial.println(TankTypeCorS);
			
					Serial.print("EEPROM_100-131: ssid: \t"); Serial.println(ssid);
					Serial.print("EEPROM_200-231: password: \t"); Serial.println(password);

					lcd.setCursor(0, 0);lcd.print("Full Tank:   ");
					if(TankTypeCorS == 0){
						FullTankLiter = ((DorAside/2)*(DorAside/2)*3.1415*(MaxTankEchodist-MinTankEchodist))/1000;
						lcd.setCursor(0, 1);lcd.print(FullTankLiter);lcd.print(" Liter          ");
					}else if(TankTypeCorS ==1){
						FullTankLiter = (DorAside*Bside*(MaxTankEchodist-MinTankEchodist))/1000;
						lcd.setCursor(0, 1);lcd.print(FullTankLiter);lcd.print(" Liter          ");
					}
					ReparameterSelection = 1;
					ProgramLevel = 2;
					ClearLCD_OneFlag = 0;
					NumberInputFlag = 0; NumberInputLevel = 1;
					delay(3000);
				break;
			}
			
        break;

      case 7: 	//Temp & Humidity --> B --> goto2
            ClearLCD_One();
        	   ReadDHT();
             ShowDHT();
            CurrentButton = ButtonHandling();
      		if('b' == CurrentButton){ //Pressing Back button
				ProgramLevel = 2;
				ClearLCD_OneFlag = 0;
			}
        
        break;

      default:
        // turn all the LEDs off:
      	break;

    }

}

void ConnectToWifi(){
  int counter = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wifi: ");
  lcd.setCursor(0, 0); lcd.print("Connecting to");
  lcd.setCursor(0, 1); lcd.print("Wifi...");


  while (WiFi.status() != WL_CONNECTED  && counter != 20) { //Connecting to Wifi | Timeout 10s
    Serial.print(".");
    counter++;
    lcd.setCursor(7, 1);
    lcd.print(counter/2);
    delay(500);
  }

  if(counter != 20){
  	Serial.println(" Success!"); 
  	Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
  	lcd.clear();
  	lcd.home();
  	lcd.print("Success!");
  	lcd.setCursor(0, 1);
  	lcd.print("SSID:");
  	String ssid_string = ssid;
  	//String messageToScroll = ssid_string; //int row, String message, int delayTime, int StartColumns, int lcdColumns)
  	//scrollText(1, messageToScroll, 500, 5, 11); //
    lcd.print(ssid);
  	WifiConnection = 1;
  	//-------------Create Wifi Signal--------------
  	//lcd.createChar(0, WifiSignal);
  	lcd.setCursor(15, 0);
  	lcd.write(0);
  	//-------------Create Wifi Signal--------------
  }else{
  	Serial.println("Timeout! Couldn't connect!"); 
  	lcd.clear();
  	lcd.home();
  	lcd.print("Timeout!");
  	lcd.setCursor(0, 1);
  	lcd.print("Couldn't connect");
  	WifiConnection = 0;
  	//-------------Create Wifi Signal--------------
  	//lcd.createChar(0, NoWifiSignal);
  	lcd.setCursor(15, 0);
  	lcd.write(4);
  	//-------------Create Wifi Signal--------------
  }
  


  //Serial.printf("Connection status: %d\n", WiFi.status());
  Serial.printf("Connection status: %s\n", WifiStatusReturn[WiFi.status()]);
  delay(3000);

  //Clear Scren exept last one
  ClearLCD_ExceptLastSymbol();
}

void CheckWifiConnection(){

  long currentMillis = millis();
  
  if (currentMillis - lastMillis_CheckWifiConnection > Period_CheckWifiConnection)
  {
  	if(WiFi.status() == WL_CONNECTED){
		WifiConnection = 1;
		//Serial.println("Wifi Connection is ok");
		//-------------Create Wifi Signal--------------
  		//lcd.createChar(0, WifiSignal);
  		//lcd.setCursor(15, 0);
  		//lcd.write(0);
  		//-------------Create Wifi Signal--------------
	}else{

		WifiConnection = 0;
		Serial.println("Wifi Connection lost...");
		//-------------Create Wifi Signal--------------
  		//lcd.createChar(0, NoWifiSignal);
  		//lcd.setCursor(15, 0);
  		//lcd.write(0);
  		//-------------Create Wifi Signal--------------
	}

    lastMillis_CheckWifiConnection = currentMillis;
  }
}

void ClearLCD_ExceptLastSymbol(){
  lcd.home();          lcd.print("               ");
  lcd.setCursor(0, 1); lcd.print("                ");
}

void SetupClock(){
  Serial.print("Setting time using SNTP. Get Time...");
  ClearLCD_ExceptLastSymbol();
  lcd.setCursor(0, 0); lcd.print("Get Time...");
  int counter = 9;
  char buffer[80];

  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
  Serial.println("\nWaiting for time");
  unsigned timeout = 5000;
  unsigned start = millis();
  while (!time(nullptr) && counter !=0)
  {
    Serial.print(".");
    
    lcd.setCursor(11, 0);
    lcd.print(counter);
    counter--;
    delay(1000);
  }
  
  ClearLCD_ExceptLastSymbol();
  
/*
  time_t rawtime;
  struct tm * timeinfo;


  //struct tm timeinfo;
  //time_t now = time(nullptr);
  //Serial.println(buffer);
  //struct tm timeinfo;
  //time_t now = time(nullptr);
  long BeginMillis = millis();
  long lastSavedMillis = millis();
  long lastSavedMillisWithinLoop = millis();
  while(lastSavedMillis < BeginMillis + 3000){
  long currentMillisWithinLoop = millis();
	  if (currentMillisWithinLoop - lastSavedMillisWithinLoop > 1000)
	  {
	  	time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (buffer,80,"%y.%m.%d %H:%M:%S",timeinfo);
	  	Serial.println(buffer);
		strftime (buffer,80,"%y.%m.%d",timeinfo);
		lcd.setCursor(0, 0);lcd.print(buffer);
		strftime (buffer,80,"%H:%M:%S",timeinfo);
		lcd.setCursor(0, 1); lcd.print(buffer);

	    lastSavedMillisWithinLoop = currentMillisWithinLoop;
	  }
  lastSavedMillis = currentMillisWithinLoop;
  delay(1);
  }
  ClearLCD_ExceptLastSymbol();
  */
}

void CheckInternetConnection(){


  long currentMillis = millis();
  
  // blink LED
  if (currentMillis - lastMillis_InternetConnection > Period_InternetCheck)
  {
  	if (!client.connect(host, httpsPort)) {
		//-------------Remove ActiveInternetConnectionAnimation--------------
		//InternetAnimationFlag = 0;
		InternetConnection = 0; //0 - Not connected to Wifi, 1 - Connected to wifi
		Serial.println("Internet Connection lost...");
		//lcd.createChar(0, WifiSignal);lcd.setCursor(15, 0);lcd.write(0);
		//-------------Remove ActiveInternetConnectionAnimation--------------
	    Serial.print("Connected to "); Serial.print(host); Serial.println(" with FAILED ");
	    return;
    }else {
		//-------------Create ActiveInternetConnectionAnimation--------------
		//ActiveInternetConnectionAnimation();
		//InternetAnimationFlag = 1;
		InternetConnection = 1; //0 - Not connected to Wifi, 1 - Connected to wifi
		//lcd.createChar(0, WifiSignal);lcd.setCursor(15, 0);lcd.write(0);
		//-------------Create ActiveInternetConnectionAnimation--------------    	
	    //Serial.print("Connected to "); Serial.print(host); Serial.println(" with success ");
	}
    lastMillis_InternetConnection = currentMillis;
  }

}



void SendDataToInternet(){
  long currentMillis = millis();
  

  if (currentMillis - lastMillis_SendDataToInternet > Period_SendDataToInternet)
  {
    char chr[4];
    snprintf(chr, 5, "%d", CurrentTankLiter);
    String str = chr;
    //String url = "https://api.thingspeak.com/update?api_key=TNXADA26CS5RF7FJ&field1=" + str  + "&field2=" + String(t) + "&field3=" + String(h) + "\r\n";
    String url = "https://api.thingspeak.com/update?api_key=FK7WA6SAHM8BELN0&field1=" + str  + "&field2=" + String(t) + "\r\n";
    //Serial.print("requesting URL: ");
    //Serial.println(url);
  
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
    //client.print(String("GET ") + url );
    
    Serial.println("request sent");

    lastMillis_SendDataToInternet = currentMillis;
  }


/*
  lcd.setCursor(0, 1);
  lcd.print("               ");
  lcd.setCursor(0, 1);
  lcd.print("Next Trigger:");
  for(int x=19;x>0;x--)
  {
    lcd.setCursor(13, 1);
    lcd.print("   ");
    lcd.setCursor(13, 1);
    lcd.print(x);lcd.print("s");
    delay(1000);
  }*/
}

void UltraSonicMeasurement(){

  long currentMillis = millis();

  if (currentMillis - lastMillis_USTime > Period_USTime)
  {
  	ValidDistance = sonar.ping_cm();
  	//ValidDistance = sonar.ping_median();
  	//Serial.print("ValidDistance: ");
	//Serial.println(ValidDistance);


  	//Fill block, if filled shift it
  	if (DistanceBlockCounter < 5 ){
  		DistanceBlock[DistanceBlockCounter] = ValidDistance;
  	}else{
  		for (int i = 0; i<4;i++){
  			DistanceBlock[i] = DistanceBlock[i+1];
  		}
  		DistanceBlock[4] = ValidDistance;
  	}
  	

  	//Filer Distance Block --> Find min
  	int min_number = 0;
  	int min_Value = DistanceBlock[0];
  	for (int i = 0;i<4;i++){
  		if (min_Value<DistanceBlock[i+1]){
  			//min_number = i;
  			//min_Value = DistanceBlock[i];
  		}
  		else{
  			min_number = i+1;
  			min_Value = DistanceBlock[i+1];
  		}
  		
  	}


    //Filer Distance Block --> Find max
  	int max_number = 0;
  	int max_Value = DistanceBlock[0];
  	for (int i = 0;i<4;i++){
  		if (max_Value>DistanceBlock[i+1]){
  			//max_number = i;
  			//max_Value = DistanceBlock[i];
  		}
  		else{
  			max_number = i+1;
  			max_Value = DistanceBlock[i+1];
  		}
  		
  	}
  	/* //Printline iformations
  	for (int i = 0;i<5;i++){
  	Serial.print(DistanceBlock[i]);Serial.print(", ");
  	}
    Serial.println("");
    Serial.print("min_Value:");Serial.print(min_Value);Serial.print(" min_number:");Serial.println(min_number);
    Serial.print("max_Value:");Serial.print(max_Value);Serial.print(" max_number:");Serial.println(max_number);  
	*/
    FileredValidDistance = 0;
    for (int i = 0;i<5;i++){
    	if(i == max_number || i == min_number){
    		//skip average calculation from min&max value
    	}
    	else{
    	FileredValidDistance = FileredValidDistance + DistanceBlock[i];
    	}
    }

    if ( max_number == min_number){
    	FileredValidDistance = FileredValidDistance/4;
    }else{
    	FileredValidDistance = FileredValidDistance/3;
    }


    //Serial.print("Not Valid echo:");Serial.print(FileredValidDistance);
    //Check  valid echo. If different from last echo more than 25cm, than need 5 more echo
    if(abs(SavedFileredValidDistance - FileredValidDistance) > EchoHysteresis && (CounterForValidEcho < NewEchoCounter)){
    	FileredValidDistance = SavedFileredValidDistance;
    	CounterForValidEcho++;
    }
    else{
    	CounterForValidEcho = 0;
    }

    SavedFileredValidDistance =	FileredValidDistance;
	//Serial.print("FileredValidDistance: ");
	//Serial.print("\t Valid echo:");Serial.println(FileredValidDistance); <<--------------------COMMENT BACK------------


  	if (DistanceBlockCounter < 5 )
  	{
  		DistanceBlockCounter++;
  	}

  //WaterLevel Calculation
  if(TankTypeCorS == 0){
    //FullTankLiter = ((DorAside/2)*(DorAside/2)*3.1415*(MaxTankEchodist-MinTankEchodist))/1000;
    CurrentTankLiter = ((DorAside/2)*(DorAside/2)*3.1415*(MaxTankEchodist-FileredValidDistance))/1000;
  }else if(TankTypeCorS ==1){
    //FullTankLiter = (DorAside*Bside*(MaxTankEchodist-MinTankEchodist))/1000;
    CurrentTankLiter = ((DorAside/2)*(DorAside/2)*3.1415*(MaxTankEchodist-FileredValidDistance))/1000;
  }

  CurrentLvLPercent = (MaxTankEchodist-FileredValidDistance)*100/(MaxTankEchodist-MinTankEchodist);


    lastMillis_USTime = currentMillis;
  }


 


// Prints the distance on the Serial Monitor

}

void ShowTime(){

 long currentMillis = millis();

  

 if (currentMillis - lastMillis_ShowTime > Period_ShowTime)
  {
	  char buffer[80];

	  //ClearLCD_ExceptLastSymbol();
	  
	  time_t rawtime;
	  struct tm * timeinfo;
	  time (&rawtime);
	  timeinfo = localtime (&rawtime);

	  //strftime (buffer,80,"%y.%m.%d %H:%M:%S",timeinfo);

	  //Serial.println(buffer);
	  strftime (buffer,80,"%y.%m.%d",timeinfo); //Year, Moount, Day
	  lcd.setCursor(0, 0);lcd.print(buffer);
	  strftime (buffer,80,"%H:%M:%S",timeinfo); //H, m, s
	  lcd.setCursor(0, 1); lcd.print(buffer);

    lastMillis_ShowTime = currentMillis;
  }

}



void scrollText(int row, String message, int delayTime, int StartColumns, int lcdColumns) {
  /*for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  }*/ 
  message = message + " "; 
  for (int pos = 0; pos < (message.length()-lcdColumns-1); pos++) {
    lcd.setCursor(StartColumns, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
  for (int pos = (message.length()-lcdColumns); pos >= 0; pos--) {
  lcd.setCursor(StartColumns, row);
  lcd.print(message.substring(pos, pos + lcdColumns));
  delay(delayTime);
  }
}

void ActiveInternetConnectionAnimation(){
  	if (InternetConnection == 1){

  	long currentMillis = millis();
	if (currentMillis - lastMillis_ActiveInternetConnectionAnimation > Period_ActiveInternetConnectionAnimation)
	    {
	    	//Serial.print("InternetConnection:");Serial.println(InternetConnection);
  			if (InternetSignalFlag == 2){
				InternetSignalFlag = 1;
  				lcd.setCursor(15, 0); 
  				//lcd.createChar(0, WifiSignal);
  				lcd.write(0);
  				//Serial.print("WifiSignal ");
  			}else if(InternetSignalFlag == 1){
  				InternetSignalFlag = 0;
  				lcd.setCursor(15, 0); 
  				//lcd.createChar(0, InternetSignal1);
  				lcd.write(1);
  				//Serial.print("InternetSignal1 ");
  			}else if(InternetSignalFlag == 0){
  				InternetSignalFlag = 2;
  				lcd.setCursor(15, 0); 
  				//lcd.createChar(0, InternetSignal2);
  				lcd.write(2);
  				//Serial.print("InternetSignal2 ");
  			}
  		
	    lastMillis_ActiveInternetConnectionAnimation = currentMillis;
	    }
	} else if(WifiConnection == 1){
		lcd.setCursor(15, 0);
		//lcd.createChar(0, WifiSignal);
  		lcd.write(0);

	}else if(WifiConnection == 0){
		lcd.setCursor(15, 0);
		//lcd.createChar(0, NoWifiSignal);
  		lcd.write(3);
	}

}

void programLevelTrack(){
	if (lastProgramLevel != ProgramLevel){
		Serial.print("ProgramLevel:");Serial.println(ProgramLevel);
		lastProgramLevel = ProgramLevel;
	}

	if (lastReparameterSelection != ReparameterSelection){
		Serial.print("ReparameterSelection:");Serial.println(ReparameterSelection);
		lastReparameterSelection = ReparameterSelection;
	}
}

char ButtonHandling(){
		int incomingByteinDec;
		char ButtonValue;
		//a-->97
		//d-->100
		//s-->115
		//w-->119
		ButtonValue ='n';

		//Serial.print("Return Button value: \t");Serial.print(ButtonValue);

		if (Button_Up_LastState != digitalRead(Button_Up)){
			Button_Up_LastState = digitalRead(Button_Up);
			Serial.print("Button_Up_Sate");Serial.println(digitalRead(Button_Up));
			if(digitalRead(Button_Up) == 0){
				ButtonValue ='u';
			}
		}
		if (Button_Ok_LastState != digitalRead(Button_Ok)){
			Button_Ok_LastState = digitalRead(Button_Ok);
			Serial.print("Button_Ok_Sate");Serial.println(digitalRead(Button_Ok));
			if(digitalRead(Button_Ok) == 0){
				ButtonValue ='o';
			}
		}
		if (Button_Down_LastState != digitalRead(Button_Down)){
			Button_Down_LastState = digitalRead(Button_Down);
			Serial.print("Button_Down_Sate");Serial.println(digitalRead(Button_Down));
			if(digitalRead(Button_Down) == 0){
				ButtonValue ='d';
			}
		}
		if (Button_Back_LastState != digitalRead(Button_Back)){
			Button_Back_LastState = digitalRead(Button_Back);
			Serial.print("Button_Back_Sate");Serial.println(digitalRead(Button_Back));
			
			if(digitalRead(Button_Back) == 0){
				ButtonValue ='b';
			}
		}

    if (Serial.available() > 0)
      {
               // read the incoming byte:
               incomingByteinDec = Serial.read();
               // say what you got:
               Serial.print("I received incomingByteinDec: \t");
               Serial.println(incomingByteinDec, DEC);
               switch (incomingByteinDec){
                case 97: //a-->97-->Back/Menu
                  ButtonValue ='b';
                  Serial.println("Serial_Button: Back");
                break;

                case 100: //d-->100-->OK
                  ButtonValue ='o';
                  Serial.println("Serial_Button: OK");
                break;

                case 115: //s-->115-->Down
                  ButtonValue ='d';
                  Serial.println("Serial_Button: Down");
                break;

                case 119: //w-->119--> Up
                  ButtonValue ='u';
                  Serial.println("Serial_Button: Up");
                break;

                default: //No current ButtonPress
                  ButtonValue ='n';
                break;

               } 
    }


		return ButtonValue;
}

void ClearLCD_One(){
	if(ClearLCD_OneFlag==0){
		ClearLCD_ExceptLastSymbol();
		ClearLCD_OneFlag=1;
	}
}

void NumberInput_3digit(){

	CurrentButton = ButtonHandling();

	if(NumberInputFlag == 0){
		lcd.setCursor(0, 1);lcd.print(num1);lcd.print(num2);lcd.print(num3);//lcd.print("cm");
		//NumberInputFlag = 1;
		lcd.blink();
	}
	

	if('u' == CurrentButton){ //Pressing Up button
		NumberInputFlag = 0;
		if (NumTemp == 9){
			NumTemp = 0;
		}else{
			NumTemp++;
		}	
	}
	if('d' == CurrentButton){ //Pressing Down button
		NumberInputFlag = 0;
		if (NumTemp == 0){
			NumTemp = 9;
		}else{
			NumTemp--;
		}
	}
	switch (NumberInputLevel){
		case 1:
			if(NumberInputFlag == 0){
				num1=NumTemp;
				lcd.setCursor(0, 1);lcd.print(num1);lcd.setCursor(0, 1);
				NumberInputFlag = 1;
			}
			if('o' == CurrentButton){ //Pressing ok button
				NumberInputLevel++; NumberInputFlag = 0;
				NumTemp=num2;
			}else if('b' == CurrentButton){

			}
		break;
		case 2:
			if(NumberInputFlag == 0){
				num2=NumTemp;
				lcd.setCursor(1, 1);lcd.print(num2);lcd.setCursor(1, 1);
				NumberInputFlag = 1;
			}
			if('o' == CurrentButton){ //Pressing ok button
				NumberInputLevel++; NumberInputFlag = 0;
				NumTemp=num3;
			}else if('b' == CurrentButton){
				NumberInputLevel--; NumberInputFlag = 0;
			}
		break;
		case 3:
			if(NumberInputFlag == 0){
				num3=NumTemp;
				lcd.setCursor(2, 1);lcd.print(num3);lcd.setCursor(2, 1);
				NumberInputFlag = 1;
			}
			if('o' == CurrentButton){ //Pressing ok button
				NumberInputLevel++;	NumberInputFlag = 0;
			}else if('b' == CurrentButton){
				NumberInputLevel--;	NumberInputFlag = 0;
			}		
		break;
		case 4:
			numberFinal = num1*100+num2*10+num3*1;
			NumberInputEnd = 1;
			NumberInputFlag = 0;
			NumberInputLevel = 1; //reset Number Input level
			lcd.noBlink();
		break;
	}
}


void ReadDHT(){
  long currentMillis = millis();
  

  if (currentMillis - lastMillis_ReadDHT > 2000)
  {
  	h = dht.readHumidity();
  	// Read the temperature as Celsius:
  	t = dht.readTemperature();
  	/*Serial.print("Humidity: ");
  	Serial.print(h);
  	Serial.print(" % ");
  	Serial.print("Temperature: ");
  	Serial.print(t);
  	Serial.println("°C");

  	lcd.setCursor(0, 0);lcd.print("Temp.:    ");lcd.print(t,1);lcd.print("C");lcd.print("        ");
  	lcd.setCursor(0, 1);lcd.print("Humidity: ");lcd.print(h,1);lcd.print("%");*/


    lastMillis_ReadDHT = currentMillis;
  }


}

void ShowDHT(){

  long currentMillis = millis();
  

  if (currentMillis - lastMillis_ShowDHT > 2000)
  {

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" % ");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println("°C");

    lcd.setCursor(0, 0);lcd.print("Temp.:    ");lcd.print(t,1);lcd.print("C");lcd.print("        ");
    lcd.setCursor(0, 1);lcd.print("Humidity: ");lcd.print(h,1);lcd.print("%");


    lastMillis_ShowDHT = currentMillis;
  }

}


void TankLvlDetectorView(){
/*
 (Tank lvl: 100% W)
 (2000/2000L 23.1C)
 */
 lcd.setCursor(0, 0); lcd.print("Tank lvl: "); lcd.print(CurrentLvLPercent);lcd.print("% ");
 lcd.setCursor(0, 1); lcd.print(CurrentTankLiter); lcd.print("/");lcd.print(FullTankLiter);lcd.print("L ");
 lcd.setCursor(11, 1);lcd.print(t,1);lcd.print("C");


}


void RawEchoShow(){

  long currentMillis = millis();
  //Write out to LCD
  if (currentMillis - lastMillis_US_LCD_Printout > Period_US_LCD_Printout)
  { 
    //lcd.setCursor(0, 0);
  //lcd.print("              ");
    
  lcd.setCursor(0, 0);
  lcd.print("Echo:");
  lcd.setCursor(9, 0);
  lcd.print("      ");
  lcd.setCursor(9, 0);
  lcd.print(FileredValidDistance);
  lcd.print("cm");

   lastMillis_US_LCD_Printout = currentMillis;
  }
}
/*
Basic Timing
  long currentMillis = millis();
  

  if (currentMillis - lastMillis_InternetConnection > Period_InternetCheck)
  {

    lastMillis_InternetConnection = currentMillis;
  }

}
*/