TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ../models

FORMS += MyForm.ui

HEADERS += MyForm.h BL3GLWidget.h MyGLWidget.h ../models/model.h

SOURCES += main.cpp  MyForm.cpp \
        BL3GLWidget.cpp MyGLWidget.cpp ../models/model.cpp
