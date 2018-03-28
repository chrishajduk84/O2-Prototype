#ifndef CARTRIDGE_H
#define CARTRIDGE_H

//#include "Heater.h"
//#include "Valve.h"
//#include "Pump.h"
//#include "TestQueue.h"
//#include "PID.h"
//#include "BangBang.h"
//#include "Sensors.h"

#define HEATER_UPDATE_PERIOD 1000
#define PUMP_UPDATE_PERIOD 1000

typedef enum _CycleState{
    INVALID = -1,
    ABSORB = 1,
    INTERMEDIATE_A = 2,
    INTERMEDIATE_B = 3,
    DESORB = 4,
    INTERMEDIATE_C = 5
} CycleState;

typedef struct _ColumnSetpoints{
    int cycles;
    float temperature;
    float inPressure;
    float outPressure;
    CycleState cycleState;
} ColumnSetpoints;

class Column{
    unsigned int cID;
    float heaterK[5] = {1000,1e-4,1,0,0};
    float pumpAK[5] = {100,1e-4,1,0,0};
    float pumpBK[5] = {-100,1e-4,1,0,0};
    long lastLoopTime = 0;
    ColumnSetpoints* cs;
    ColumnSetpoints* initialCS;

    public:
    //Heater heater;
    //PID<Heater> heaterPID;
    //Valve vA; //2 Way
    //Valve vB; //2 Way
    //Valve vC; //3 Way
    //Pump pA;    //Pressure
    //PID<Pump> pumpAPID;
    //Pump pB;    //Vacuum
    //PID<Pump> pumpBPID;
    //Sensors cartridgeSensors; //All Sensors
    //TestQueue tQueue;
    //static Cartridge* getById(unsigned int id);
    Column(unsigned int id);
    ~Column();
    int getHeatingTime();
    int getCoolingTime();
    int getStateTime();
    int getCycleTime();
    int getCycle();
    float getTemperature();
    float getPressure(); 
    ColumnSetpoints* getSetpoints();
    void updateSetpoints(ColumnSetpoints* _cs);
    //void setTestQueue(TestQueue* tq);
    //void update();
    //Test getCurrentTest();
};

#endif
