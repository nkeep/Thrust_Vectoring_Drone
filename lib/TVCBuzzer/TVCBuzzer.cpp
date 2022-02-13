#include <Arduino.h>
#include <TVCBuzzer.h>

TVCBuzzer::TVCBuzzer(int pin){
    this->pin = pin;
}

void TVCBuzzer::playIndianaJones(){
    int frequencies[33] = {0, 0, 329, 349, 392, 523, 293, 329, 
								349, 392, 440, 493, 698, 440, 493, 
								523, 587, 659, 329, 349, 392, 523, 587, 659,
								698, 392, 392, 659, 587, 392, 659, 587, 392};
    
    int durations[33] = {1017, 508, 381, 127, 254, 1271, 381, 127, 
								1525, 381, 127, 254, 1271, 381, 127, 
								508, 508, 508, 381, 127, 254, 1271, 381, 127,
								1525, 381, 127, 508, 381, 127, 508, 381, 127};

    for(int i = 0; i < 33; i++){
        tone(this->pin, frequencies[i]);
        delay(durations[i]);
        noTone(this->pin);
        Serial.print(frequencies[i]);
        //noTone(this->pin);
    }

}

