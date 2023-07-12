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

void ConnectToWifi(uint16_t timeout);

void setup() {
  Serial.begin(115200);

ConnectToWifi(2000);
ArduinoOTA
    .onStart([]() {
      //esp_task_wdt_init(6000, true); //enable panic so ESP32 restarts
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
  WiFi.mode(WIFI_OFF);//WifiStatus = false;

  Serial.println("Setup done");
}

void loop() {
if (Serial.available() > 0)
  {
    char input = Serial.read();
if (input == 'h')
    {
      //WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      //WifiStatus = false;
    }
    if (input == 'j')
    {
      ConnectToWifi(5000);
    }
  }


delay(1);
}


void ConnectToWifi(uint16_t timeout){
    WiFi.mode(WIFI_STA);
    //WifiStatus = true;
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