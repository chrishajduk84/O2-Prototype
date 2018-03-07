#include "MainWindow.h"
#include <QTimer>
#include <QImage>
#include <stdio.h>

//Images:
#include "img/DownArrow.h"
#include "img/UpArrow.h"

MainWindow::MainWindow(QWidget* parent):QWidget(parent){
     
    //ADD VIVASPIRE LOGO
    QTimer::singleShot(10, this, SLOT(showFullScreen()));
    
    //Up/Down Arrows
    
    const QImage upArrow(UpArrowData,457,257,QImage::Format_RGB888);
    const QImage downArrow(UpArrowData,257,457,QImage::Format_RGB888);//257,457
    QPixmap upArrowPM = QPixmap::fromImage(upArrow);//("img/UpArrow.png");
    QPixmap downArrowPM = QPixmap::fromImage(downArrow);//("img/DownArrow.png");
    //upArrowPM.convertFromImage(upArrow);
    //downArrowPM.convertFromImage(downArrow);
    QIcon upButtonIcon(upArrowPM);
    QIcon downButtonIcon(downArrowPM);
    up = new QPushButton("" ,this);
    up->setGeometry(50,40,60,60);
    up->setIcon(upButtonIcon);
    up->setIconSize(QSize(50,50));//upArrow.rect().size());
    down = new QPushButton("", this);
    down->setGeometry(50,140,60,60);
    down->setIcon(downButtonIcon);
    down->setIconSize(QSize(50,50));

    //Flow Rate
    flowrate = new QLabel("1 LPM", this);
    QFont qf1 = flowrate->font();
    qf1.setPointSize(20); //This may be too big for the raspberrypi
    flowrate->setFont(qf1);
    flowrate->setGeometry(125,36,210,60);

    //Oxygen Concentration
    oxygenfrac = new QLabel("92%", this);    
    QFont qf2 = oxygenfrac->font();
    qf2.setPointSize(20);
    oxygenfrac->setFont(qf2);
    oxygenfrac->setGeometry(220,36,90,60);
}

void MainWindow::updateO2(float percentage){
	char o2String[5];
	sprintf(o2String,"%.1f%%",percentage);
	oxygenfrac->setText(o2String);
}
