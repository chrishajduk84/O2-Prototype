#include "Valve.h"
#include "PWM.h"
#include <stdlib.h>

Valve::Valve(unsigned int _togglePin) : pwmInterface(_togglePin){
    if (_togglePin >= 0){
              
        
    }
}

Valve::~Valve(){}

void Valve::toggle(){
    toggle(!state);
    state = !state;
}

void Valve::toggle(bool _state){
    if (_state == state) return; //No need to redo this function if the desired state is already in place
  
    if (_state){ //ON
        pwm.setDuty(100);
        long int tempTime = myMillis();
        long int diff = 0;
	std::thread([=](){std::this_thread::sleep_for(std::chrono::seconds(n)); pwm.setDuty(50);}).detach(); //Might have trouble compiling this line.
    }
    else{ //OFF
        pwm.setDuty(0);
    }
    state = _state;
}

bool Valve::getState(){
    return state;
}


