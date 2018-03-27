#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include "Column.h"

#define NUM_COLUMN 3
#define ABSORPTION 1
#define INTERMEDIATE_A 2
#define INTERMEDIATE_B 3
#define DESORPTION 4
#define INTERMEDIATE_C 5


//Maybe move somewhere else
typedef struct _ColumnState{
    int HeatingTime;
    int htAverage;
    int htVariance;
    int CoolingTime;
    int ctAverage;
    int ctVariance;
    int nMeasurements;
    int cycle;
} ColumnState;

class Synchronizer{
    
    public:
    static Synchronizer* getInstance();
    void addColumn(int index, Column* c);
    void update();

    private:
    Column* cList[NUM_COLUMN];
    ColumnState previousState[NUM_COLUMN];
    static Synchronizer* s_instance;
    int phaseSetpoint[NUM_COLUMN] = {0,120,240}; //Degrees - Temporarily hard-coded
    int currentPeriod = 0; //The time it takes for all 3 columns to reach their initial state (EX: if column 1 is twice as fast as column 2, it will the common period is the longer of the two or a multiple thereof.);

    Synchronizer(){}
    Synchronizer(Synchronizer const&);              // Don't Implement
    void operator=(Synchronizer const&); // Don't implement
        
};



#endif
