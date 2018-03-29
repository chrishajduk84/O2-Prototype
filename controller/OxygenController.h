#ifndef OXYGENCONTROLLER_H
#define OXYGENCONTROLLER_H

#include <QObject>
#include <thread>
//#include "Sensors.h"
#include "Synchronizer.h"

class OxygenController : public QObject{
    Q_OBJECT
    
    public:
    OxygenController();
    ~OxygenController();
    
    private:  
    void runLoop();

    signals:
        void updateOxygen(float o2);
        void updateFlow(float flow);
};

#endif
