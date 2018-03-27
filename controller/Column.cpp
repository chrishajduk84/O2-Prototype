#include "Column.h"
#include <stdlib.h>

/*Column* Column::getById(unsigned int id){
    if (id > 0)
        return cList[id-1];
    else
        return 0;
}*/

Column::Column(unsigned int id){
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
}

int Column::getHeatingTime(){
    return 0;
}

int Column::getCoolingTime(){
    return 0;
}

int Column::getCycle(){
    return 0;
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
}

void Column::update(){
/*    //Update Sensor Data
    cartridgeSensors.updateSensors();
    
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
    lastLoopTime = myMillis();
}*/

