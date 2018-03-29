#ifndef COLUMN_H
#define COLUMN_H

#include "ThermalRelay.h"
//#include "Valve.h"
//#include "Pump.h"
//#include "TestQueue.h"
//#include "PID.h"
//#include "BangBang.h"
#include "Sensors.h"
#include <chrono>

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
    std::chrono::milliseconds stateTime = std::chrono::milliseconds(0);
    std::chrono::milliseconds cycleTime = std::chrono::milliseconds(0);
    std::chrono::milliseconds heatTime = std::chrono::milliseconds(0);
    std::chrono::milliseconds coolTime = std::chrono::milliseconds(0); 
    std::chrono::milliseconds columnUpdateTime = std::chrono::milliseconds(0);
    std::chrono::system_clock::time_point lastUpdateTimePoint;
    CycleState lastCycleState = INVALID;

    ColumnSetpoints* cs;
    ColumnSetpoints* initialCS;

    public:
    Sensors cSensors;
    ThermalRelay heater;
    ThermalRelay cooler;
    //PID<Heater> heaterPID;
    //Valve vA; //2 Way
    //Valve vB; //2 Way
    //Valve vC; //3 Way
    //Pump pA;    //Pressure
    //PID<Pump> pumpAPID;
    //Pump pB;    //Vacuum
    //PID<Pump> pumpBPID;
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
    void update();
    //Test getCurrentTest();
};

#endif
