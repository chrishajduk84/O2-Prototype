#include "GPIO.h"

    GPIO::GPIO(int pinNumber){

    }
    GPIO::~GPIO(){


    }
    void GPIO::toggle(bool _state){

        state = _state;
    }
    void GPIO::toggle(){
        state = !state;

    }
    bool GPIO::getState(){
        return state;
    }
    void GPIO::setPWM(int _duty){
        //Check for existing threads
        //Save thread handler

    }
    void GPIO::stopPWM(){
        //Stop thread
        
    }
