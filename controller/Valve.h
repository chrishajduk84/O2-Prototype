#ifndef VALVE_H
#define VALVE_H

#include "PWM.h"
#include "OxygenController.h"

//static void* delayedValvePWM(void*);

class Valve{
    unsigned int togglePin;
    bool state = false;
    PWM pwmInterface; 

    public:
    Valve(unsigned int _togglePin);
    ~Valve();
    void toggle();
    void toggle(bool _state);
    bool getState();
};

#endif
