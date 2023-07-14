#include "MyQLCDNumber.h"

// constructor
MyQLCDNumber::MyQLCDNumber(QWidget *parent):QLCDNumber(parent) 
{
    display(time.toString("hh:mm:ss.zzz"));
    connect(&timer, SIGNAL(timeout()), this, SLOT(feina()));
}

void MyQLCDNumber::start() {
    if(!timer.isActive()) {
        timer.start(1);
        feina();
    }

    else {
        timer.stop();

        if(first) {
            rec = time;
            first = false;
        }
        
        if(time <= rec) {
            rec = time;
            emit enviaTime(rec.toString("hh:mm:ss.zzz"));
        }
    }
    
}

void MyQLCDNumber::feina() {
        time = time.addMSecs(1);
        display(time.toString("hh:mm:ss.zzz"));
        if(rec != QTime(0,0,0,0)) {
            if(time < rec) setStyleSheet("color: green;");
            else if(time == rec) setStyleSheet("color: black;");
            else if(time > rec) setStyleSheet("color: red;");
        }
}

void MyQLCDNumber::restart() {
    time = QTime(0,0,0,0);
    display(time.toString("hh:mm:ss.zzz"));
}



