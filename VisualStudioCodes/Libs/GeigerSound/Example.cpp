
#include <Energia.h>
#include <stdint.h>
#include <Geiger.h>
/*
--------PIN DEFINITIONS----------
*/
//Outputs
#define BUZZER  P2_5
//Analog inputs
#define POT     P1_0
//Digital inputs

/*
--------GLOBAL VARIABLES----------
*/
/*
--------FUNCTION DECLARATION----------
*/



void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(POT,    INPUT);

  Serial.begin(9600),
  Serial.println("Setup Done!");
}

void loop() {
  Geiger(BUZZER,map(analogRead(POT),0,1023,0,100));
}
