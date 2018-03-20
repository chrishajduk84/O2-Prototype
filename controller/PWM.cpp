#include "PWM.h"
#include <iostream>

#define I2C_PWM 0x15

#define SET_ON_OFF 0x14
#define MODE1 0x00
#define MODE2 0x01

using namespace std;

    static int numPWMConnection = 0; 
    static int pwmFD = 0;    
    PWM::PWM(int pinNumber){
        if (numPWMConnection == 0){
            pwmFD = wiringPiI2CSetup(I2C_PWM); 
            //Reset chip to blank state
            if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF,0) < 0) cout << "[I2C] Error sending PWM update";
            if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF+1,0) < 0) cout << "[I2C] Error sending PWM update";
            if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF+2,0) < 0) cout << "[I2C] Error sending PWM update";
            if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF+3,0) < 0) cout << "[I2C] Error sending PWM update";
            //Turn off sleep mode
            if (wiringPiI2CWriteReg8(pwmFD, MODE1,0b00000000) < 0) cout << "[I2C] Error sending PWM update";
            //Use Open Drain circuit (not Totem-Pole)
            if (wiringPiI2CWriteReg8(pwmFD, MODE2,0b00010101) < 0) cout << "[I2C] Error sending PWM update";

        }

        numPWMConnection++;
        pin = pinNumber;

    }
    
    PWM::~PWM(){
        numPWMConnection--;
        if (numPWMConnection == 0){
            //TODO: close I2C port
        }
    }
    void PWM::toggle(bool _state){
        long int currentState = wiringPiI2CReadReg8(pwmFD, SET_ON_OFF) + (wiringPiI2CReadReg8(pwmFD, SET_ON_OFF + 1) << 8) +  ((long int)wiringPiI2CReadReg8(pwmFD, SET_ON_OFF + 1) << 16) + ((long int)wiringPiI2CReadReg8(pwmFD, SET_ON_OFF + 3) << 24);
        currentState &= ~(_state << (pin*2+1));
        currentState |= (_state << (pin*2+1));
        if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF, (int)(currentState & 0xFF)) < 0) cout << "[I2C] Error sending PWM update";
        if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF+1, (int)((currentState & 0xFF00) >> 8)) < 0) cout << "[I2C] Error sending PWM update";
        if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF+2, (int)(currentState & 0xFF0000) >> 16) < 0) cout << "[I2C] Error sending PWM update";
        if (wiringPiI2CWriteReg8(pwmFD, SET_ON_OFF+3, (int)((currentState & 0xFF000000) >> 24)) < 0) cout << "[I2C] Error sending PWM update";


        state = _state;
    }
    void PWM::toggle(){
        state = !state;
        toggle(state);
    }
    bool PWM::getState(){
        long int currentState = wiringPiI2CReadReg8(pwmFD, SET_ON_OFF) + (wiringPiI2CReadReg8(pwmFD, SET_ON_OFF+1) << 8) + (wiringPiI2CReadReg8(pwmFD, SET_ON_OFF+2) << 16) + (wiringPiI2CReadReg8(pwmFD, SET_ON_OFF+3) << 24);
        state = currentState & (1 << (pin*2 + 1));
        return state;
    }

    int PWM::getDuty(){
        return wiringPiI2CReadReg8(pwmFD, pwmArray[pin]);
    }

    void PWM::setDuty(int _duty){
        _duty = (int)(255*_duty/100.0); //convert from 0 - 100, to 0 to 0xFF
        wiringPiI2CWriteReg8(pwmFD, pwmArray[pin], _duty);
        duty = _duty;
    }
