#include "OxygenController.h"
#include <QFuture>
#include <QThread>
#include <qtconcurrentrun.h>

#include "GPIO.h"
#include "ThermalRelay.h"
#include "Column.h"
#include <iostream>

using namespace std;

OxygenController::OxygenController(){
    Column* c0 = new Column(0);
    Column* c1 = new Column(1);
    Column* c2 = new Column(2);
    Synchronizer* s_instance = Synchronizer::getInstance();
    s_instance->addColumn(0,c0);
    s_instance->addColumn(1,c1);
    s_instance->addColumn(2,c2);

    //Start Worker - Main control thread
    QFuture<void> controlThread = QtConcurrent::run(this, &OxygenController::runLoop);

    GPIO powerLED(10);
    powerLED.toggle(true);

}

OxygenController::~OxygenController(){
    
}

void OxygenController::runLoop(){
    while(true){ 
        //Todo: Make these static methods in the Sensors object
        //Update LCD Display using Slots and Signals:
        //updateFlow(c0Sensor.getSensorData()->flow);
        //updateOxygen(c0Sensor.getSensorData()->O2);
        updateFlow(0);
        updateOxygen(0); 

        //Run Cycles
        //-Will need a way to synchronize the columns(dependent on feedback)
        //-Each synchronous column controls its own pumps,valves, heater and cooler to achieve synchronizer setpoint
        //*For now we will use a simple synchronizer
        Synchronizer::getInstance()->update(); //Recursively updates columns 
        /*if ( ){ //break condition - power button?   
            
        }*/
    }
}
