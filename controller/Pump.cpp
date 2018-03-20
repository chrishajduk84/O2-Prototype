#include "Pump.h"
#include <stdlib.h>

//NOTE: See Heater.cpp for ISR routine

//These are static!!!
unsigned int Pump::pinList[NUM_CARTRIDGES*2];
unsigned int Pump::listLength;
Pump* Pump::pumpList[NUM_CARTRIDGES*2];

Pump::Pump(unsigned int _togglePin):pumpGPIO(_togglePin){
    //Assign the pump pin to the object
    for (int i = 0; i < listLength; i++){
        if (pinList[i] == _togglePin){
            exit(1);
        }
    }
    //If the pump pin hasn't already been initialized, add it to the list
    pumpList[listLength] = this;
    pinList[listLength++] = togglePin = _togglePin; 
}

Pump::~Pump(){
    //Remove all entries in static variables
    bool removed = false;
    for (int i = 0; i < listLength; i++){
        if (pinList[i] == togglePin){
            removed = true;
        }
        if (removed){
            if (i+1 >= listLength) pinList[i] = 0;
            else pinList[i] = pinList[i+1];
        }
    }
}
   
void Pump::toggle(){
    state = !state;
    pumpGPIO.toggle(state); 
}

void Pump::toggle(bool _state){
    pumpGPIO.toggle(_state);
    state = _state;
}

bool Pump::getState(){
    return pumpGPIO.getState();
}

//TODO: IMPLEMENT LATER
void Pump::setPWM(int _duty){ //Hacks not really (hardware) PWM
  pwm = true;
  duty = _duty;
}

void Pump::stopPWM(){ //Hacks not really PWM
  pwm = false;
}
