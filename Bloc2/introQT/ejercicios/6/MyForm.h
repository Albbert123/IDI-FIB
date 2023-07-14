#include "ui_MyForm.h" //el .h del Myform.ui

class MyForm : public QWidget
{

    Q_OBJECT //macro de c++ que hay q ponerlo en a clases de tipo widget

    public:
    MyForm(QWidget *parent = 0);

    private:
    Ui::MyForm ui; //object name de qt designer
};