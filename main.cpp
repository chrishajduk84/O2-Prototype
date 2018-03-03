#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}
