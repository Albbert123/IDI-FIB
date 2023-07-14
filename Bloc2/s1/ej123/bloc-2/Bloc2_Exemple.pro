TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm /home2/users/alumnes/1277946/dades/IDI/b2/s1/bloc-2/Model

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h /home2/users/alumnes/1277946/dades/IDI/b2/s1/bloc-2/Model/model.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp /home2/users/alumnes/1277946/dades/IDI/b2/s1/bloc-2/Model/model.cpp
