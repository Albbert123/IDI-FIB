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
    virtual void modelTransformPatricio ();
    virtual void calculaCapsaModel ();

    GLuint posFocusLoc;
    GLuint posFocusLoc2;
    GLuint colorFocusLoc;
    GLuint colorFocusLoc2;
    GLuint llumAmbientLoc;
    GLuint focusEscenaLoc;

    glm::vec3 colorFocus, colorFocus2, posFocus, posFocus2, llumAmbient;


    float angleX = 0.0;
    int focusEsc; //bool
    float moux=0,mouz=0;

    bool apaga1 = false;
    bool apaga2 = false;

    bool cul = false;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
