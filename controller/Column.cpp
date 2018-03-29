#include "Column.h"
#include "Pinout.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

/*Column* Column::getById(unsigned int id){
    if (id > 0)
        return cList[id-1];
    else
        return 0;
}*/

Column::Column(unsigned int id) : cSensors(id), heater(heaterLocation[id].pinNumber), cooler(coolerLocation[id].pinNumber){
    //Starting conditions
    initialCS = cs = new ColumnSetpoints; 
    cs->cycles = 0;
    cs->temperature = 0;
    cs->inPressure = 0;
    cs->outPressure = 0;
    cs->cycleState = ABSORB;
     
    lastUpdateTimePoint = chrono::system_clock::now(); 

    /*//Assign a reference in a static array
    if (id <= NUM_CARTRIDGES){
        if (!cList[id-1]){
            cList[id-1] = this;
        }
        else{
            cout << "Initialized Column Already Exists!" << endl;
        }

        heaterPID.setSensorSource(&cartridgeSensors.getSensorData()->temperature);
        heaterPID.setOutput(&heater,&heater.setPWM);
        heaterPID.setGain(heaterK);
        pumpAPID.setSensorSource(&cartridgeSensors.getSensorData()->pInlet); //Positive Pressure Sensor
        pumpAPID.setOutput(&pA,&pA.setPWM);
        pumpAPID.setGain(pumpAK);
        pumpBPID.setSensorSource(&cartridgeSensors.getSensorData()->pOutlet);//Absolute Pressure Sensor
        pumpBPID.setOutput(&pB,&pB.setPWM);
        pumpBPID.setGain(pumpBK);
        cID = id;       
        //Do things with the queue?
    }
    */
}

Column::~Column(){
    delete initialCS;
}

int Column::getHeatingTime(){
    return heatTime.count();
}

int Column::getCoolingTime(){
    return coolTime.count();
}

int Column::getStateTime(){
    return stateTime.count();
}

int Column::getCycleTime(){
    return cycleTime.count();   
}

int Column::getCycle(){
    return cs->cycles;
}

float Column::getTemperature(){
    return cSensors.getAvgTemperature();
}

float Column::getPressure(){
    return 0;
}


ColumnSetpoints* Column::getSetpoints(){
    return cs;           
}

void Column::updateSetpoints(ColumnSetpoints* _cs){
    cs = _cs;
}

/*void Column::setTestQueue(TestQueue* tq){
  tQueue = *tq;
  if (tQueue.size() > 0){
    heaterPID.setOutput(&heater,&heater.setPWM);
    pumpAPID.setOutput(&pA,&pA.setPWM);
    pumpBPID.setOutput(&pB,&pB.setPWM);
    currentTest = tQueue.getCurrentTest();
    heaterPID.setSetpointSource(&currentTest->getTestSetpoints()->temperature);
    pumpAPID.setSetpointSource(&currentTest->getTestSetpoints()->inPressure);
    pumpBPID.setSetpointSource(&currentTest->getTestSetpoints()->outPressure);
    heater.setMaxPower(currentTest->getTestParameters()->heatingPower);
  }
}*/

void Column::update(){
    //Update Time parameters - Time since beginning of cycle, time since beginning of state, time since heating began, time since cooling began
    columnUpdateTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - lastUpdateTimePoint);
    lastUpdateTimePoint = chrono::system_clock::now();
    stateTime += columnUpdateTime;
    cycleTime += columnUpdateTime;

    if (cs->cycleState == INTERMEDIATE_C) coolTime += columnUpdateTime;
    if (cs->cycleState == INTERMEDIATE_A || cs->cycleState == INTERMEDIATE_B) heatTime +=columnUpdateTime;       

    //Update Sensor Data - DO WE NEED THIS HERE? Or update each sensor as needed? Polling?
    //cSensors.updateSensors();

    if (lastCycleState != cs->cycleState){ //Only run this if statement section if things have changed  (state-wise)
        lastCycleState = cs->cycleState;
        stateTime -= stateTime; //reset to 0
        if (cs->cycleState == ABSORB){
            heater.toggle(false);
            cooler.toggle(true);
        } 
        else if (cs->cycleState == INTERMEDIATE_A){
            heater.toggle(true);
            cooler.toggle(false);
        }
        else if (cs->cycleState == INTERMEDIATE_B){
            heater.toggle(true);
            cooler.toggle(false);
        }
        else if (cs->cycleState == DESORB){
            heater.toggle(true);
            cooler.toggle(false);
        }
        else if (cs->cycleState == INTERMEDIATE_C){
            heater.toggle(false);
            cooler.toggle(true);       
        }   

    }
/*    
    //If all tests have finished put the device into a safe state 
    if (tQueue.size() <= 0){ //If the test queue is empty, put the cartridge in to a safe state and return
      pumpAPID.toggle(false); pumpBPID.toggle(false);heaterPID.toggle(false);
      pA.stopPWM();pB.stopPWM();heater.stopPWM();
      pA.toggle(false);pB.toggle(false);heater.toggle(false);
      vA.toggle(false);vB.toggle(false);vC.toggle(false);
      return; 
    }
    //Check if current Test has finished
    currentTest = tQueue.getCurrentTest();
    if (!currentTest->update(cartridgeSensors.getSensorData())){
      delete tQueue.pop(); //Delete Previous Test
      currentTest = tQueue.getCurrentTest(); //Start new test
      //if (!currentTest) return; //If the test queue is empty, return
      
      //and switch control systems
      heaterPID.setSetpointSource(&currentTest->getTestSetpoints()->temperature);
      pumpAPID.setSetpointSource(&currentTest->getTestSetpoints()->inPressure);
      pumpBPID.setSetpointSource(&currentTest->getTestSetpoints()->outPressure);
      heater.setMaxPower(currentTest->getTestParameters()->heatingPower);
    }
    //Update Control Systems
    heaterPID.update(myMillis() - lastLoopTime);
    pumpAPID.update(myMillis() - lastLoopTime);
    pumpBPID.update(myMillis() - lastLoopTime);
    
    if (currentTest->getTestSetpoints()->cycleState == ABSORB){
      vA.toggle(false);
      vB.toggle(false);   //This should be PWM'd to set backpressure
      vC.toggle(false);
      pumpAPID.toggle(true);
      pumpBPID.toggle(false);
    }
    else if (currentTest->getTestSetpoints()->cycleState == INTERMEDIATE_A){
      vA.toggle(true);
      vB.toggle(false);   //This should be PWM'd to set backpressure
      vC.toggle(false);
      pumpAPID.toggle(false);
      pumpBPID.toggle(true);
    }
    else if (currentTest->getTestSetpoints()->cycleState == INTERMEDIATE_B){
      vA.toggle(true);
      vB.toggle(true);   //This should be PWM'd to set backpressure
      vC.toggle(false);
      pumpAPID.toggle(false);
      pumpBPID.toggle(false);
    } 
    else if (currentTest->getTestSetpoints()->cycleState == DESORB){
      vA.toggle(true);
      vB.toggle(false);   //This should be PWM'd to set backpressure
      vC.toggle(true);
      pumpAPID.toggle(false);
      pumpBPID.toggle(true);
    } else{

    }
    lastLoopTime = myMillis(); */
}

