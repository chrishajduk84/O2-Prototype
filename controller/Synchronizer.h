#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include "Column.h"

#define NUM_COLUMN 3
#define HEATING_EPSILON 0.1

typedef struct _columnSettings{
    float absorbTemp; //Default Value: 45
    float desorpTemp; //Default Value: 90C
    float inPressure; //Default Value: 8 psi - NOT CONTROLLED IN INITIAL PROTOTYPE (REV 3.0)
    float outPressure; //Default Value: 8 psi
    float heatingPower; //Default Value: 5A*14.4V = 72W
    int maxHeatingTime; //Default Value: 2*60 seconds
    int maxCoolingTime; //Default Value: 2*60 seconds
    int minHeatingTime; //Default Value: 1*60 seconds
    int minCoolingTime; //Default Value: 1*60 seconds
    float flow; //Default Value: 0.5 LPM - NOT CONTROLLED (FOR NOW)
} ColumnSettings;

//Maybe move somewhere else
typedef struct _ColumnAnalysis{
    int HeatingTime;
    int htAverage;
    int htVariance;
    int CoolingTime;
    int ctAverage;
    int ctVariance;
    int nMeasurements;
    int cycle;
} ColumnAnalysis;

class Synchronizer{
    
    public:
    static Synchronizer* getInstance();
    void addColumn(int index, Column* c);
    void update();

    private:
    Column* cList[NUM_COLUMN];
    ColumnAnalysis previousState[NUM_COLUMN];
    ColumnSettings mSettings = {45, 90, 0, 0, 1.0,2*60*1000/5,2*60*1000/5,1*60*1000/5,1*60*1000/5,0.5};
    static Synchronizer* s_instance;
    int phaseSetpoint[NUM_COLUMN] = {0,120,240}; //Degrees - Temporarily hard-coded
    int currentPeriod = 0; //The time it takes for all 3 columns to reach their initial state (EX: if column 1 is twice as fast as column 2, it will the common period is the longer of the two or a multiple thereof.);

    Synchronizer(){}
    Synchronizer(Synchronizer const&);              // Don't Implement
    void operator=(Synchronizer const&); // Don't implement 
};

#endif
