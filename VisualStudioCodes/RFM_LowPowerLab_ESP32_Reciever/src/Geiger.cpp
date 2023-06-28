#include <Geiger.h>

void Ping(uint8_t Pin, uint8_t volume){
  /*
  Creates the single pulse for the iconic Geiger counter sound
  Inputs: The pin that is connected to the buzzer
  */
  //Does not stop the CPU but a bit more quiet
  /*
  static  uint32_t OldTime;
          uint32_t NewTime  = 0;
      
  NewTime = millis();  
  digitalWrite(Pin,1);
  if (NewTime - OldTime >= 1 ) {
    OldTime = NewTime;
    digitalWrite(Pin,0);
  }
  */
  //Does stop the CPU but for some reason more loud...
  
  //digitalWrite(Pin,1);
  analogWrite(Pin,volume);
  delay(1);
  analogWrite(Pin,0);
  
}
void Geiger(uint8_t Pin,uint16_t Intensity, uint8_t volume){
    /*
    Creates the geiger sound using the buzzer on Pin, with the given Intesity. The intesity could be given between 0-100. 
    */
    
    static  uint32_t OldTime;
    static  bool     Flag;
            bool     Off;
            uint32_t NewTime  = 0;
    static  uint16_t WaitTime = 0;
    
    NewTime = millis(); 
    //Serial.println(Intensity);
    if (Intensity == 0){
      Off = true;
    } else {
      Off = false;
    }

    if (!Off){
      if (Flag){
        Intensity = map(Intensity,0,100,1000,0);
        WaitTime  = rand() % Intensity + (Intensity * 0.5);
        //Serial.println(WaitTime);
        Flag = false;
      }
      if (NewTime - OldTime >= WaitTime) {
        OldTime = NewTime;
        Ping(Pin, volume);
        Flag = true;
      }
    }
}