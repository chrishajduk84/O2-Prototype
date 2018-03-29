#include "Synchronizer.h"
#include <iostream>

Synchronizer* Synchronizer::s_instance;

using namespace std;

Synchronizer* Synchronizer::getInstance(){
    if(!s_instance){
        s_instance = new Synchronizer;   
    }
    return s_instance;
}

void Synchronizer::addColumn(int index, Column* c){
   cList[index] = c;
}

void Synchronizer::update(){
    int calcPeriod = 0;
    for (int i=0; i < NUM_COLUMN; i++){
        cList[i]->update(); //Update each column - controls of valves/pumps/etc.
        //Update Sync data
        if (previousState[i].cycle < cList[i]->getCycle() ){ //Only update predictions when necessary
            //HEATING TIME
            cout << "HEATING" << endl;
            previousState[i].HeatingTime = cList[i]->getHeatingTime();
            int newHeatingAverage = (previousState[i].htAverage*previousState[i].nMeasurements)+previousState[i].HeatingTime;
            previousState[i].htVariance = previousState[i].htVariance + (previousState[i].HeatingTime-previousState[i].htAverage)*(previousState[i].HeatingTime - newHeatingAverage); //This is the incremental variance equation
            previousState[i].htAverage = newHeatingAverage;

            //COOLING TIME
            previousState[i].CoolingTime = cList[i]->getCoolingTime();
            int newCoolingAverage = (previousState[i].ctAverage*previousState[i].nMeasurements)+previousState[i].CoolingTime;
            previousState[i].ctVariance = previousState[i].ctVariance + (previousState[i].CoolingTime-previousState[i].ctAverage)*(previousState[i].CoolingTime - newCoolingAverage); //This is the incremental variance equation
            previousState[i].ctAverage = newCoolingAverage;
            
            //OTHER
            previousState[i].cycle = cList[i]->getCycle();
            previousState[i].nMeasurements++;
            
            //Calculate current period
            calcPeriod += (previousState[i].HeatingTime + previousState[i].CoolingTime);
        }
        if (calcPeriod != currentPeriod) calcPeriod = currentPeriod;

        //ABSORPTION/DESORPTION MODEL TO BE USED SOMEWHERE HERE
        //The model should probably calculate the mSettings variable and the phase should modify it to keep all systems in sync 
        //Decide when and where to switch states. Also need to change setpoints to increase/decrease phase
        //This problem is to control the phase in addition to the rest of everything.
        //Eventually the phase setpoint needs to change based on the output, if one column has subpar performance, then it should have a smaller phase band. 
        
        //TODO: Generate mSettings

        for (int i = 0; i < NUM_COLUMN; i++){
            ColumnSetpoints* cs = cList[i]->getSetpoints();
            cout << cList[i]->getTemperature() << endl;
            if (cs->cycleState == ABSORB){ 
                //SWITCHING CONDITIONS - At the end of the absorption state switch to the intermediate state
                if (((cList[i]->getTemperature() <= cs->temperature) && (cList[i]->getStateTime() >= mSettings.minCoolingTime)) || cList[i]->getStateTime() >= mSettings.maxCoolingTime){ //+- PhaseControl (add it to minCoolingTime, to extend Period to match other columns)
                    cs->cycleState = INTERMEDIATE_A;
                    //Update Setpoints for INTERMEDIATE_A - Temperature, Pressure
                    cs->temperature = mSettings.desorpTemp;
                    cs->outPressure = mSettings.outPressure;
                    cs->inPressure = 0;
                    cList[i]->updateSetpoints(cs);
                }
            }
            else if (cs->cycleState == INTERMEDIATE_A){ //The purpose of this is to pump down the cartridge
                cout << "IA" << endl;
                if (cList[i]->getPressure() <= cs->outPressure){
                    cs->cycleState = INTERMEDIATE_B;
                    //Update Setpoints for INTERMEDIATE_B - Temperature, Pressure
                    cs->temperature = mSettings.desorpTemp;
                    cs->outPressure = mSettings.outPressure;
                    cs->inPressure = 0;
                    cList[i]->updateSetpoints(cs);
                }
            }
            else if (cs->cycleState == INTERMEDIATE_B){ //The purpose of this is to seal it until it is ready to desorb (temperature-wise)
                cout << "IB" << endl;
                if (cList[i]->getTemperature() >= cs->temperature - HEATING_EPSILON){
                    cs->cycleState = DESORB;
                    //Update Setpoints for DESORPTION - Temperature, Pressure
                    cs->temperature = mSettings.desorpTemp;
                    cs->outPressure = mSettings.outPressure;
                    cs->inPressure = 0;
                    cList[i]->updateSetpoints(cs);
                }
            }
            else if (cs->cycleState == DESORB){
                cout << "D" << endl;
                //SWITCHING CONDITIONS - At the end of the desorption state, switch to absorption
                if (((cList[i]->getTemperature() >= cs->temperature - HEATING_EPSILON) && (cList[i]->getStateTime() >= mSettings.minHeatingTime)) || cList[i]->getStateTime() >= mSettings.maxHeatingTime){
                    cs->cycleState = INTERMEDIATE_C;
                    //Update Setpoints for INTERMEDIATE_C - Temperature, Pressure
                    cs->temperature = mSettings.absorbTemp;
                    cs->inPressure = mSettings.inPressure;
                    cs->outPressure = 14.5;
                    cList[i]->updateSetpoints(cs);
                }
            }
            else if (cs->cycleState == INTERMEDIATE_C){
                cout << "IC" << endl; 
                //SWITCHING CONDITIONS - At the end of the desorption state, switch to absorption
                if (((cList[i]->getTemperature() <= cs->temperature - HEATING_EPSILON) && (cList[i]->getStateTime() >= mSettings.minHeatingTime)) || cList[i]->getStateTime() >= mSettings.maxHeatingTime){
                    cs->cycles++;
                    cs->cycleState = ABSORB;
                    //Update Setpoints for ABSORBTION - Temperature, Pressure
                    cs->temperature = mSettings.absorbTemp;
                    cs->inPressure = mSettings.inPressure;
                    cs->outPressure = 14.5;
                    cList[i]->updateSetpoints(cs);
                }
            }
            else {
                //Update Setpoints - Start with absorption state
                cout << "ELSE" << i << endl;
                cs->cycleState = ABSORB;
                cList[i]->updateSetpoints(cs);
            }
        }
    }    
} 
