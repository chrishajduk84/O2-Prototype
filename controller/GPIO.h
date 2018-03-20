#ifndef GPIO_H
#define GPIO_H

#include "OxygenController.h"
#include <wiringPiI2C.h>

class GPIO{
    bool state = false;
    int pin = 0;

    public:
    GPIO(int pinNumber);
    ~GPIO();
    void toggle(bool _state);
    void toggle();
    bool getState();
    void setPWM(int _duty);
    void stopPWM();
};



#endif
