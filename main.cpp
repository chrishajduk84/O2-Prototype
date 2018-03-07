#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <QApplication>
#include <unistd.h>
#include <thread>

#include "gui/MainWindow.h"
#include "controller/Sensors.h"

using namespace std;

int main(int argc, char** argv){
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show(); 
    Sensors test(1);

    mw.updateO2(test.getO2());
    //TODO: Add way of concurrently running "controller"	

    return app.exec();
}
