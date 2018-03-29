#include "ThermalRelay.h"
#include <stdlib.h>
#include <iostream>

//These are static!!!
unsigned int ThermalRelay::pinList[NUM_CARTRIDGES*2];
unsigned int ThermalRelay::listLength;
ThermalRelay* ThermalRelay::relayList[NUM_CARTRIDGES*2];

/* 
unsigned long int timer = 0;
ISR(TIMER5_OVF_vect)   // timer compare interrupt service routine
{
  timer += 1;
  /************HEATER****************
  for (int i = 0; i < ThermalRelay::listLength; i++){
    if (ThermalRelay::relayList[i]->pwm){
      if (timer%100 < ((ThermalRelay::relayList[i]->duty)*ThermalRelay::relayList[i]->heatingFactor)){
        ThermalRelay::relayList[i]->toggle(true);
      }
      else{
        ThermalRelay::relayList[i]->toggle(false);
      }
    }
  }
  
  /************PUMP****************
  for (int i = 0; i < Pump::listLength; i++){
    if (Pump::pumpList[i]->pwm){
      if (timer%100 < Pump::pumpList[i]->duty){
        Pump::pumpList[i]->toggle(true);
      }
      else{
        Pump::pumpList[i]->toggle(false);
      }
    }
  }
}
/****************************/

ThermalRelay::ThermalRelay(unsigned int _togglePin):thermalGPIO(_togglePin){
    //Assign the heating pin to the object
    for (int i = 0; i < listLength; i++){
        if (pinList[i] == _togglePin){
            std::cout << "Runtime Error: Reinstantiation of GPIO:" << _togglePin << std::endl;
            exit(1);
        }
    }
    //If the heating pin hasn't already been initialized, add it to the list
    relayList[listLength] = this;
    pinList[listLength++] = togglePin = _togglePin;
}

ThermalRelay::~ThermalRelay(){
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

void ThermalRelay::toggle(){
    state = !state;
    toggle(state);
}

void ThermalRelay::toggle(bool _state){
    thermalGPIO.toggle(_state);
    state = _state;
}

bool ThermalRelay::getState(){
    thermalGPIO.getState();
    return state;
}

void ThermalRelay::setPWM(int _duty){ //Hacks not really (hardware) PWM
  pwm = true;
  duty = _duty;
}

void ThermalRelay::stopPWM(){ //Hacks not really PWM
  pwm = false;
}

void ThermalRelay::setMaxPower(float _heatingFactor){
  heatingFactor = _heatingFactor;
}

