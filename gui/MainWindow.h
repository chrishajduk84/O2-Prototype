#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QWidget{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = 0);
    signals:
        public slots:
    
    private:
    QPushButton* up;
    QPushButton* down;
    QLabel* flowrate;
    QLabel* oxygenfrac;
};

#endif
