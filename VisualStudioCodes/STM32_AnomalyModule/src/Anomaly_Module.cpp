#include <stdint.h>
#include <Filter.h>
#include <Arduino.h>
/*
--------PIN DEFINITIONS----------
*/
//Outputs
#define RED_OUT     PA0
#define GREEN_OUT   PA1
#define BLUE_OUT    PA2
//Analog inputs
#define RED_LVL     PA3
#define GREEN_LVL   PA4 //in final version uart is not needed P1_1 can be used
#define BLUE_LVL    PA5 //in final version uart is not needed P1_2 can be used
#define FREQ_IN     PA6 //1_3
#define PARAM_IN    PA7 //1_4
//Digital inputs
#define MODE1       PB0
#define MODE2       PB1
#define LED         PC13
/*
--------STATE DEFINES----------
*/
#define NORMAL  0
#define STROBE  1
#define PULSE   2
#define DEBUG   3
/*
--------GLOBAL VARIABLES----------
*/
uint16_t  color[3] = {0xFF,0xFF,0xFF};
Filter    RFilt, GFilt,BFilt,FreqFilt,ParamFilt;
uint8_t   Flag = 0;
/*
--------FUNCTION DECLARATION----------
*/

uint8_t   Read_State();
void      Read_Color(uint16_t color[3]);
void      Write_Color(uint16_t color[3],float intensity = 1);
void      LED_off();
void      State_Strobe(uint16_t Freq, uint16_t Param);
void      State_Pulse(uint16_t Freq, uint16_t Param);
void      State_Normal(uint16_t Freq, uint16_t Param);
void      State_Debug();


void setup() {
    //SET UP PINS
    pinMode(RED_OUT   ,OUTPUT);
    pinMode(GREEN_OUT ,OUTPUT);
    pinMode(BLUE_OUT  ,OUTPUT);
    pinMode(LED       ,OUTPUT);

    pinMode(RED_LVL   ,INPUT);
    pinMode(GREEN_LVL ,INPUT);
    pinMode(BLUE_LVL  ,INPUT);
    pinMode(FREQ_IN   ,INPUT);
    pinMode(PARAM_IN  ,INPUT);
    pinMode(MODE1     ,INPUT);
    pinMode(MODE2     ,INPUT);

    delay(1);
    //INIT OUTPUTS
    digitalWrite(RED_OUT,0);
    digitalWrite(GREEN_OUT,0);
    digitalWrite(BLUE_OUT,0);
    //SET UP UART - not needed in final version
    Serial.begin(115200);
    Serial.println("Setup done!");
}
void loop() {
  //LOCAL VARIABLES
  //Serial.println("Running");
  Read_Color(color);
  uint16_t Freq   = FreqFilt.MovingAvg(analogRead(FREQ_IN)); //Read inputs and filter them
  uint16_t Param  = ParamFilt.MovingAvg(analogRead(PARAM_IN));
  
  switch (Read_State()){ // normal working
  //switch (DEBUG){ //testing different states  
  
    case NORMAL:
      State_Normal(Freq,Param);
    break;

    case STROBE:
      State_Strobe(Freq,Param);
    break;
    
    case PULSE:
      State_Pulse(Freq,Param);
    break;
    
    case DEBUG:
      State_Debug();
    break;

    default:
      State_Normal(Freq,Param);
    break;
  }
}
void State_Strobe(uint16_t Freq, uint16_t Param){
    // LOCAL VARIABLES
    unsigned  long      currentTime;
    static    uint32_t  previousMillis;

    float   Duty    = Param / 1023.0; //Calculte parameters
            Freq    = Freq * 2;

    currentTime = millis();

    if (currentTime - previousMillis >= Freq) {
        previousMillis = currentTime;
        digitalWrite(LED, 1);
        Write_Color(color);
    }
    if (currentTime - previousMillis >= Freq * Duty){ 
        digitalWrite(LED,0);
        LED_off();
    }
}
void State_Pulse(uint16_t Freq, uint16_t Param){
    // LOCAL VARIABLES
    unsigned  long      t_1; // Current time
    static    uint32_t  t_0; // Cycle start time
              uint16_t  t_c,t_cont,t_up;
              float     intensity;

    float   Ramp    = Param / 1023.0; //Calculte parameters
            Freq    = Freq * 2;
            t_1     = millis();
            t_c     = t_1 - t_0;        //Cycle current time
            t_cont  = Freq * (1-Ramp);  //Continous light time
            t_up    = Freq * Ramp / 2;  //Dimming time

    if (t_1 - t_0 >= Freq) {
        t_0 = t_1;
    } // Cycle restart

    if (t_c > t_up + t_cont){ //Counting down et the end
      intensity = 1.0 - (((float)t_c - ((float)t_cont + (float)t_up)) / (float)t_up);
    } else if ( t_c < t_up){ //Counting up in the beggining
      intensity = (float)t_c / (float)t_up;
    } else{ // the middle part
      intensity = 1;
    }
    Write_Color(color,intensity);
  }
void State_Normal(uint16_t Freq, uint16_t Param){
    float Intensity = Param/1023.0;
    Write_Color(color,Intensity);
}
void State_Debug(){
    Serial.print("Mode1: "); Serial.println(digitalRead(MODE1));
    Serial.print("Mode2: "); Serial.println(digitalRead(MODE2));
    //Read_State();
    Read_Color(color);
    Serial.print("Color values: [R,G,B] "); 
    Serial.print(color[0]); Serial.print(" , "); Serial.print(color[1]); Serial.print(" , "); Serial.println(color[2]);

    Serial.print("Value: "); Serial.println(analogRead(PARAM_IN) );
    Serial.print("Freq: "); Serial.println(analogRead(FREQ_IN));
    delay(100);
}
uint8_t Read_State(){ //This vould be lot more nicer if done by interrupst...
    //Function reads the input switch for selectin mode and 
    uint8_t ret = 0;
    uint8_t mode_1 = digitalRead(MODE1);
    uint8_t mode_2 = digitalRead(MODE2);

    if (mode_1 == 1 && mode_2 == 1){
      ret = 0;
    } else if (mode_1 == 1 && mode_2 == 0){
      ret = 1;
    } else if (mode_1 == 0 && mode_2 == 1){
      ret = 2;
    }
    return ret;
}
void Read_Color(uint16_t color[]){
    color[0] = RFilt.MovingAvg(analogRead(RED_LVL));
    color[1] = GFilt.MovingAvg(analogRead(GREEN_LVL));
    color[2] = BFilt.MovingAvg(analogRead(BLUE_LVL));
    
    for (uint8_t i = 0; i < 3; i++){
      color[i] = map(color[i],0,1023,0,255);
      //Serial.println(i);
    }
    /*
    Serial.print(color[0]); Serial.print("  ");
    Serial.print(color[1]); Serial.print("  ");
    Serial.print(color[2]); Serial.println("  ");
    */
}
void Write_Color(uint16_t color[3],float intensity){
    for (uint8_t i = 0; i < 3; i++){
      color[i] = (uint16_t)(color[i] * intensity);
    }  
    analogWrite(RED_OUT   ,color[0]);
    analogWrite(GREEN_OUT ,color[1]);
    analogWrite(BLUE_OUT  ,color[2]);
/*
    Serial.print(color[0]); Serial.print("  ");
    Serial.print(color[1]); Serial.print("  ");
    Serial.print(color[2]); Serial.println("  ");
*/
}
void LED_off(){
    analogWrite(RED_OUT   ,0);
    analogWrite(GREEN_OUT ,0);
    analogWrite(BLUE_OUT  ,0);  
}