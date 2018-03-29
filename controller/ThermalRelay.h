#ifndef HEATER_H
#define HEATER_H

#include "GPIO.h"

#define NUM_CARTRIDGES 3

class ThermalRelay{
    static unsigned int pinList[NUM_CARTRIDGES*2];
    unsigned int togglePin;
    bool state;
    GPIO thermalGPIO;

    public:
    bool pwm = false;
    int duty;
    float heatingFactor = 1;
    static unsigned int listLength;
    static ThermalRelay* relayList[NUM_CARTRIDGES*2];
    ThermalRelay(unsigned int _togglePin);
    ~ThermalRelay();
    void toggle();
    void toggle(bool _state);
    bool getState();
    void setPWM(int _duty); //Hacks not really PWM
    void stopPWM(); //Hacks not really PWM
    void setMaxPower(float heatingFactor);
};

#endif
