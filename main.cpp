#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <QFuture>
#include <QApplication>
#include <unistd.h>
#include <thread>

#include "gui/MainWindow.h"
#include "controller/Sensors.h"
#include "controller/OxygenController.h"

using namespace std;

int main(int argc, char** argv){
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();

    OxygenController oc;
 
    QObject::connect(&oc, SIGNAL(updateOxygen(float)), &mw, SLOT(updateO2(float)), Qt::QueuedConnection);
    QObject::connect(&oc, SIGNAL(updateFlow(float)), &mw, SLOT(updateFlow(float)), Qt::QueuedConnection);

    //TODO: Add a way to shutdown controller after application closes

    return app.exec();
}
