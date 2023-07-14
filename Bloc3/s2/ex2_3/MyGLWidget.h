// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);

    // virtual void paintGL();
    virtual void initializeGL();
    virtual void iniMaterialTerra ();
    virtual void carregaShaders ();
    virtual void inillum();
    virtual void viewTransform ();

    GLuint posFocusLoc;
    GLuint colorFocusLoc;
    GLuint llumAmbientLoc;
    GLuint focusEscenaLoc;

    glm::vec3 colorFocus, posFocus, llumAmbient;


    float angleX = 0.0;
    int focusEsc; //bool

  private:
    int printOglError(const char file[], int line, const char func[]);
};