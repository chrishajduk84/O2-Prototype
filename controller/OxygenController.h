#ifndef OXYGENCONTROLLER_H
#define OXYGENCONTROLLER_H

#include <QObject>
#include <thread>
#include "Sensors.h"

class OxygenController : public QObject{
    Q_OBJECT
    
    public:
    OxygenController();
    ~OxygenController();
    

    private:  
    Sensors c1Sensor;
    //Sensors c2Sensor;
    //Sensors c3Sensor;
    
    void runLoop();

    signals:
        void updateOxygen(float o2);
        void updateFlow(float flow);
};

#endif
