#include <Startup.h>
#include <Arduino.h>



   // defines for the frequency of the notes (.5 x freq of mid C)
#define AN    220     // 440 Hz
#define AS    233     // 466 Hz
#define BN    247     // 493 Hz
#define CN    261     // 523 Hz
#define CS    277     // 554 Hz
#define DN    294     // 588 Hz
#define DS    311     // 622 Hz
#define EN    330     // 658 Hz
#define FN    349     // 698 Hz
#define FS    370     // 740 Hz
#define GN    392     // 784 Hz
#define GS    415     // 830 Hz
// defines for the duration of the notes (in ms)
#define wh    1024
#define h      512
#define dq     448
#define q      256
#define qt     170
#define de     192
#define e      128
#define et      85
#define oo7      1    // 007 jingle

void play_tune(int tune, uint8_t pin){               // play a tune . . .
  switch (tune) {                       // a case for each tune 
  case 1:                             // 007  E, F#, G (mult by 2 for next higher octave)
    ToneOut(EN*2,qt, pin);                    
    ToneOut(FS*2,qt, pin);
    ToneOut(GN*2,qt, pin);
    ToneOut(GN*2,qt, pin);
    ToneOut(FS*2,qt, pin);
    ToneOut(EN*2,qt, pin);
    ToneOut(EN*2,qt, pin);                    
    ToneOut(FS*2,qt, pin);
    ToneOut(GN*2,qt, pin);
    ToneOut(GN*2,qt, pin);
    ToneOut(FS*2,qt, pin);
    ToneOut(EN*2,qt, pin);
    delay(h);
    break;
  }
}

void ToneOut(int pitch, int duration, uint8_t buzzerPin){  // pitch in Hz, duration in ms
  int delayPeriod;
  long cycles, i;

  //pinMode(PIEZO_PIN, OUTPUT);           // turn on output pin
  delayPeriod = (500000 / pitch) - 7;   // calc 1/2 period in us -7 for overhead
  cycles = ((long)pitch * (long)duration) / 1000; // calc. number of cycles for loop

  for (i=0; i<= cycles; i++){           // play note for duration ms 
    analogWrite(buzzerPin, 100); 
    delayMicroseconds(delayPeriod);
    analogWrite(buzzerPin, 0); 
    delayMicroseconds(delayPeriod - 1); // - 1 to make up for digitaWrite overhead
  }
  //pinMode(PIEZO_PIN, INPUT);            // shut off pin to avoid noise from other operations
}




