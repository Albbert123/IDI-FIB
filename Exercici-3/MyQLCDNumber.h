#include <QLCDNumber>
#include <QTimer>
#include <QTime>
class MyQLCDNumber: public QLCDNumber
{

    Q_OBJECT 

    public:

    MyQLCDNumber (QWidget *parent);

    QTimer timer;
    QTime time = QTime(0,0,0,0);
    QTime rec = QTime(0,0,0,0);
    bool first = true;


    public slots:
    void start();
    void restart();
    void feina();

    signals:
    void enviaTime(QString);

};