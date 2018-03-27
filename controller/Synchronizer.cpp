#include "Synchronizer.h"

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
    currentPeriod = 0;
    for (int i=0; i < NUM_COLUMN; i++){
        
        //Update Sync data
        if (previousState[i].cycle < cList[i]->getCycle() ){ //Only update predictions when necessary
            //HEATING TIME
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
            calcPeriod += (previousState[i].HeatingTime + previous[i].CoolingTime);
        }
        if (calcPeriod != currentPeriod) calcPeriod = currentPeriod;
        

        //ABSORPTION/DESORPTION MODEL TO BE USED SOMEWHERE HERE
        
        //Decide when and where to switch states. Also need to change setpoints to increase/decrease phase
        //This problem is to control the phase in addition to the rest of everything.
        //Eventually the phase setpoint needs to change based on the output, if one column has subpar performance, then it should have a smaller phase band. 
        /*
        for (int i = 0; i < NUM_COLUMN; i++){
                    //Update Setpoints for INTERMEDIATE_B - Temperature, Pressure
                    mSetpoints.temperature = mSettings.desorpTemp;
                    mSetpoints.outPressure = mSettings.outPressure;
                    mSetpoints.inPressure = 0;
                    //Reset Timer
                    beginStateTime = myMillis()/1000.0;
                }
            }
            else if (mSetpoints.cycleState == INTERMEDIATE_B){ //The purpose of this is to seal it until it is ready to desorb (temperature-wise)
                if (sensorData->temperature >= mSetpoints.temperature - HEATING_EPSILON){
                    mSetpoints.cycleState = DESORB;
                    //Update Setpoints for DESORPTION - Temperature, Pressure
                    mSetpoints.temperature = mSettings.desorpTemp;
                    mSetpoints.outPressure = mSettings.outPressure;
                    mSetpoints.inPressure = 0;
                    //Reset Timer
                    beginStateTime = myMillis()/1000.0;
                }
            }
            else if (mSetpoints.cycleState == DESORB){
                //SWITCHING CONDITIONS - At the end of the desorption state, switch to absorption
                if (((sensorData->temperature >= mSetpoints.temperature - HEATING_EPSILON) && (mData.stateTime >= mSettings.minHeatingTime)) || mData.stateTime >= mSettings.maxHeatingTime){
                    mSetpoints.cycles++;
                    mSetpoints.cycleState = ABSORB;
                    //Update Setpoints for ABSORBTION - Temperature, Pressure
                    mSetpoints.temperature = mSettings.absorbTemp;
                    mSetpoints.inPressure = mSettings.inPressure;
                    mSetpoints.outPressure = 14.5;
                    //Reset Timer
                    beginStateTime = myMillis()/1000.0;
                }
            }
            else {
                beginStateTime = myMillis()/1000.0;
                //Update Setpoints - Start with absorption state
                mSetpoints.cycleState = ABSORB;
            }
            mData.stateTime = myMillis()/1000.0 - beginStateTime;
        }
        */
    }    
} 

