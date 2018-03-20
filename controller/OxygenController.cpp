#include "OxygenController.h"
#include <QFuture>
#include <QThread>
#include <qtconcurrentrun.h>

#include "GPIO.h"
#include "PWM.h"
#include <iostream>

using namespace std;

OxygenController::OxygenController() : c1Sensor(1){
    QFuture<void> controlThread = QtConcurrent::run(this, &OxygenController::runLoop);
    PWM p(0);
    p.setDuty(25);
    p.toggle(true);
    p.getState();
}

OxygenController::~OxygenController(){

}

void OxygenController::runLoop(){
    while(true){
        c1Sensor.updateSensors();
        updateFlow(c1Sensor.getSensorData()->flow);
        updateOxygen(c1Sensor.getSensorData()->O2);
         
        
        /*if ( ){ //break condition - power button?   
            
        }*/
    }
}
