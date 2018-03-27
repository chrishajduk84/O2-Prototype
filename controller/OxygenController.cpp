#include "OxygenController.h"
#include <QFuture>
#include <QThread>
#include <qtconcurrentrun.h>

#include "GPIO.h"
#include "PWM.h"
#include "Valve.h"
#include "Pump.h"
#include "ThermalRelay.h"
#include <iostream>

using namespace std;

OxygenController::OxygenController() : c0Sensor(0), c1Sensor(1), c2Sensor(2){
    QFuture<void> controlThread = QtConcurrent::run(this, &OxygenController::runLoop);
    
    Column c0(0);
    Column c1(1);
    Column c2(2);
    Synchronizer* s_instance = Synchronizer::getInstance();
    s_instance->addColumn(0,c0);
    s_instance->addColumn(1,c1);
    s_instance->addColumn(2,c2);
    /*
    Valve p(0);
    p.toggle(true);


    //Pump p1(6);
    //p1.toggle(true);

    ThermalRelay tr0(0);
    tr0.toggle(true);
    ThermalRelay tr1(1);
    tr1.toggle(true);
    ThermalRelay tr2(2);
    tr2.toggle(true);
    ThermalRelay tr3(3);
    tr3.toggle(true);
    ThermalRelay tr4(4);
    tr4.toggle(true);
    ThermalRelay tr5(5);
    tr5.toggle(true);*/

    GPIO powerLED(10);
    powerLED.toggle(true);

}

OxygenController::~OxygenController(){
    
}

void OxygenController::runLoop(){
    while(true){
        c0Sensor.updateSensors();
        //c1Sensor.updateSensors();
        //c2Sensor.updateSensors();

        //Update LCD Display using Slots and Signals:
        updateFlow(c0Sensor.getSensorData()->flow);
        updateOxygen(c0Sensor.getSensorData()->O2);
         
        //Run Cycles
        //-Will need a way to synchronize the columns(dependent on feedback)
        //-Each synchronous column controls its own pumps,valves, heater and cooler to achieve synchronizer setpoint
        //*For now we will use a simple synchronizer
        //mSynchronizer.update(); //Recursively updates columns
               
        /*if ( ){ //break condition - power button?   
            
        }*/
    }
}
