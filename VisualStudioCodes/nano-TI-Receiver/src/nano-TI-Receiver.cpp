#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <TM1637Display.h>
#include <Filter.h>

/* PIN DEFINITIONS*/
#define OUTPUT_PIN  3 // Buzzer pin
#define CLK         5 // Display pins
#define DIO         6 // Display pins

/* GLOBAL VARIABLES*/
int RSSI  = 0;
int LQI   = 0;
int loss  = 0;

byte buffer[61] = {0};

volatile bool     pulseRequested  = false;
volatile uint8_t  geigerIntensity = 0;
volatile uint32_t elapsedMs       = 0;
volatile uint16_t CPM;
volatile uint32_t LastPingTime    = 0;     // Utolsó detektálás ideje
volatile uint16_t waitTime        = 1000;      // Két Ping közötti idő
volatile bool     geigerActive    = false; // Geiger engedélyezve
volatile uint8_t  geigerPin;            // Buzzer pin

/* FUNCTION DECLARATION*/
void      DisplayUpdater();
void      setupPingTimer();
void      setupGeigerTimer();
void      triggerPulse();
void      Geiger(uint8_t Pin, uint16_t Intensity);
uint16_t  CalculateCPM(uint16_t WaitTime);
uint8_t   RSSI_to_Intensity(int RSSI);
void      setupDisplay();
void      setupRadio();
void      displayCPM(uint16_t cpm);
void      displayDots();
void      serialDebug();

/* GLOBAL OBJECTS */
TM1637Display display(CLK, DIO);
Filter        CPMfilt(8);

/* CONSTANTS */
const uint8_t SEG_OL[] = {  // Overload sign
  SEG_G,                                           // - 
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_D | SEG_E | SEG_F,                           // L
	SEG_G                                            // -
	};
  const uint8_t SEG_Err[] = {  // Overload sign
  SEG_A| SEG_D | SEG_E | SEG_F | SEG_G,  // E 
	SEG_E | SEG_G ,                        // r
	SEG_E | SEG_G ,                        // r
	0                                   // -
	};

void setup(){

    Serial.begin(9600);
    pinMode(3, OUTPUT);
    randomSeed(analogRead(A0));
    setupRadio();
    setupPingTimer();
    setupGeigerTimer();
    interrupts(); // Globális megszakítások engedélyezése    

    display.clear();
    display.setBrightness(0x0f);
}
void loop(){
    uint16_t intens;
    static unsigned long LastMsg = 0;
    static int rec_msg_counter = 0;
    
    //Checks whether something has been received.
    //When something is received we give some time to receive the message in full.(time in millis)
    if (ELECHOUSE_cc1101.CheckRxFifo(100)){
        if (ELECHOUSE_cc1101.CheckCRC()){    //CRC Check. If "setCrc(false)" crc returns always OK!
        rec_msg_counter++;
        RSSI = ELECHOUSE_cc1101.getRssi();
        LQI = ELECHOUSE_cc1101.getLqi();
        int len = ELECHOUSE_cc1101.ReceiveData(buffer);
        buffer[len] = '\0';
        LastMsg = millis();
        //display.showNumberDec(RSSI, false);         
        }
    }
    if (millis() - LastMsg >= 600) {
        loss++;
        //Serial.println("no msg");
        LQI = 0;
        RSSI = 0;
        //display.clear();
        displayDots();
    }
    intens = RSSI_to_Intensity(RSSI);
    CPM = CalculateCPM(waitTime);
    displayCPM(CPM);
    Geiger(OUTPUT_PIN, intens);
    //serialDebug();
}
void setupPingTimer() {
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12); // CTC mód
  TCCR1B |= (1 << CS11) | (1 << CS10); // Előosztó: 64

  OCR1A = 250; // 1 ms = 250 * 4 us

  TIMSK1 |= (1 << OCIE1A); // Compare Match A megszakítás engedélyezése
}
void setupGeigerTimer() {
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2A |= (1 << WGM21); // CTC mód
  TCCR2B |= (1 << CS22) | (1 << CS20); // Előosztó: 128 → 16 MHz / 128 = 125 kHz → 1 tick = 8 us

  OCR2A = 125; // 1 ms = 125 * 8 us

  TIMSK2 |= (1 << OCIE2A); // Compare Match A megszakítás engedélyezése
}
void triggerPulse() {
  pulseRequested = true;
  digitalWrite(geigerPin, HIGH);
  TCNT1 = 0; // Timer1 számláló nullázása
}
ISR(TIMER1_COMPA_vect) {
  if (pulseRequested) {
    digitalWrite(geigerPin, LOW);
    LastPingTime = millis(); // Frissítjük az utolsó detektálást
    pulseRequested = false;
  }
}
ISR(TIMER2_COMPA_vect) {
  if (!geigerActive) return;

  elapsedMs++;

  if (elapsedMs >= waitTime) {
    elapsedMs = 0;
    triggerPulse(); // Impulzus kiadása

    // Új véletlen várakozási idő számítása az aktuális intenzitás alapján
    uint16_t baseTime = map(geigerIntensity, 0, 100, 1000, 5);
    waitTime = random(baseTime * 0.5, baseTime + baseTime * 0.5);
  }
}
void Geiger(uint8_t Pin, uint16_t Intensity) {
  if (Intensity == 0) {
    geigerActive = false; // Leállítás
    return;
  }
  if (Intensity > 100){
    Intensity = 100;
  }
  
  geigerPin = Pin;
  geigerIntensity = Intensity;
  geigerActive = true;
}
uint16_t CalculateCPM(uint16_t WaitTime) {
    if (WaitTime == 0) return 0; // Nullával osztás védelme
    if (millis() - LastPingTime > 1000) {
    return 0;
    }   
    return 60000 / WaitTime;
}
uint8_t RSSI_to_Intensity(int RSSI) {
  const int16_t RSSI_min = -80;
  const int16_t RSSI_max = -30;
  uint8_t ret;
  if (RSSI == 0) return 0;
  
  if (RSSI < RSSI_min) {
    ret = 0;
  }else if (RSSI > RSSI_max){
    ret = 100;
  } else {
    ret = map(RSSI, RSSI_min, RSSI_max, 0, 99);
  }
  return ret;
  //intens = map(RSSI,-95,-20,0,98);
}
void setupRadio(){
    if (ELECHOUSE_cc1101.getCC1101()){         // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
    }else{
    Serial.println("Connection Error");
    display.setSegments(SEG_OL);
    delay(50000);
    }
    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setCrc(1);      // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    Serial.println("Rx Mode");
}
void displayCPM(uint16_t cpm){
  static uint64_t last;
  
  if (cpm > 9500){
    display.setSegments(SEG_OL);
  } else {
    if (millis() - last >= 500) {
      last = millis();
      display.showNumberDec(CPM, false); 
      //display.showNumberDec(CPMfilt.MovingAvg(CPM), false); 
    }
  }
}
void displayDots(){
  static uint8_t  k;
  static uint64_t last;

  if (millis() - last >= 500) {
      last = millis();
      k++;
      if (k > 3) k = 0;
      display.showNumberDecEx(0, (0x80 >> k), false);
  }
}
void serialDebug(){
  Serial.print("T:");Serial.print(millis()/1000);
  Serial.print("  Rssi: ");
  Serial.print(RSSI);
  Serial.print("  LQI: ");
  Serial.print(LQI);
  Serial.print("  Message: ");
  Serial.println((char *) buffer);
  Serial.print("   Estimated CPM: ");
  Serial.println(CPM);  
}