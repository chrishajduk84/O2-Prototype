#include "GPIO.h"
#include <iostream>

#define I2C_GPIO 0x42

#define SET_PIN_DIRECTION 0x14
#define SET_PIN_STATE 0x12
#define READ_PIN_STATE 0x10

using namespace std;

    static int numConnection = 0; 
    static int fd = 0;    
    GPIO::GPIO(int pinNumber){
        if (numConnection == 0){
            fd = wiringPiI2CSetup(I2C_GPIO);
            //Set all pins as output:
            if (wiringPiI2CWriteReg16(fd, SET_PIN_DIRECTION, 0xFFFF) < 0){
                cout << "[I2C] Error setting pin state" << endl;
            }
        }
        numConnection++;
        pin = pinNumber;

        //Determine initial state - is it on or off?
        wiringPiI2CReadReg16(fd, READ_PIN_STATE) & (1 << pinNumber)?state = true:state = false;
    }
    
    GPIO::~GPIO(){
        numConnection--;
        if (numConnection == 0){
            //TODO: close I2C port
        }
    }
    void GPIO::toggle(bool _state){
        int readAddress = READ_PIN_STATE;
        int writeAddress = SET_PIN_STATE;
        /*//Decide if LSB or MSB
        if (pin > 7){
            readAddress += 1;
            writeAddress += 1;
        }*/
        int currentState = wiringPiI2CReadReg16(fd, readAddress);
        currentState ^= (_state << pin);
        wiringPiI2CWriteReg16(fd, writeAddress, currentState);
        state = _state;
    }
    void GPIO::toggle(){
        state = !state;
        int currentState = wiringPiI2CReadReg16(fd, READ_PIN_STATE);
        currentState ^= (state << pin);
        wiringPiI2CWriteReg16(fd, SET_PIN_STATE, currentState);
    }
    bool GPIO::getState(){
        return state;
    }

/* TODO: Software PWM, Later
    void GPIO::setPWM(int _duty){
        //Check for existing threads
        //Save thread handler

    }
    void GPIO::stopPWM(){
        //Stop thread
        
    }
    */
