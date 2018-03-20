#include "Valve.h"
#include <stdlib.h>
#include <qtconcurrentrun.h>
#include <QThread>
#include <QTimer>
#include <thread>

#include <iostream>

using namespace std;

Valve::Valve(unsigned int _togglePin) : pwmInterface(_togglePin){
    if (_togglePin >= 0){
        pwmInterface.toggle(true);                     
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
        pwmInterface.setDuty(100);
        
       // std::thread([&](){cout << "2" << endl;std::this_thread::sleep_for(std::chrono::milliseconds(2000));cout << "IWAITED" << endl;refPWMInterface->setDuty(50);cout << "LATER" << endl;}).detach();
        this_thread::sleep_for(chrono::milliseconds(100));
        pwmInterface.setDuty(50);
    }
    else{ //OFF
        pwmInterface.setDuty(0);
    }
    state = _state;
}

bool Valve::getState(){
    return state;
}


//static void* delayedValvePWM(void*){sleep(1);pwmInterface.setDuty(50);}
