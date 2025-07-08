
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Arduino.h>
#include <LowPower.h>

#define BATT_PIN A6

byte transmitt_byte[11] = {72,101,108,108,111,32,87,111,114,108,100};
char *transmitt_char = "Hello World";
int num =0;
uint16_t battVoltage;

void RadioSetup();
uint16_t readBattVoltage(int pin);
void splitToBytes(uint16_t value, uint8_t &highByte, uint8_t &lowByte);

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("Nano1-Sender---Start--");
    Serial.println();
    RadioSetup();
}

void loop() {
byte highByte;
byte lowByte;
byte transmitt_data[3] = {72,101,108};

//battVoltage = readBattVoltage(BATT_PIN);

battVoltage = 3654; //dummy adat teszteléshez
splitToBytes(battVoltage,  highByte, lowByte);

transmitt_data[0] = lowByte;
transmitt_data[1] = highByte;
transmitt_data[2] = 2;

ELECHOUSE_cc1101.SendData(transmitt_data, 3, 2);
LowPower.idle(SLEEP_250MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);


}

void RadioSetup(){
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
uint16_t readBattVoltage(int pin){
  uint16_t adcValue = analogRead(pin);
  uint32_t voltage = (uint32_t)adcValue * 5000 * 2 / 1023;
  
  return (uint16_t)voltage;

}
void splitToBytes(uint16_t value, uint8_t &highByte, uint8_t &lowByte) {
  highByte = (value >> 8) & 0xFF; // felső 8 bit
  lowByte  = value & 0xFF;        // alsó 8 bit
}