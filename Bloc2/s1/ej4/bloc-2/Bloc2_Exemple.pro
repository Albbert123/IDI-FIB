TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm /home/albert/Documentos/UPC/Q4/IDI/Bloc2/s1/ej4/bloc-2/Model

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h /home/albert/Documentos/UPC/Q4/IDI/Bloc2/s1/ej4/bloc-2/Model/model.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp /home/albert/Documentos/UPC/Q4/IDI/Bloc2/s1/ej4/bloc-2/Model/model.cpp
