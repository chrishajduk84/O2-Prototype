#ifndef PWM_H
#define PWM_H

#include <wiringPiI2C.h>

class PWM{
    bool state = false;
    int duty = 0;
    int pin = 0;
    char pwmArray[16] = {0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xA,0xB,0xC,0xD,0xE,0xF,0x10,0x11};

    public:
    PWM(int pinNumber);
    ~PWM();
    void toggle(bool _state);
    void toggle();
    bool getState();
    int getDuty();
    void setDuty(int _duty);
};



#endif
