#include <QApplication>
#include "MyForm.h"

int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    MyForm form;
    form.show();
    return app.exec();
}

//1,cambiar mombre del objeto
//2. hacer el ultimo layout al objeto entero
//3. fer els signals