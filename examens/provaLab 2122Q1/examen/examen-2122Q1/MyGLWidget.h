#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void canvicam1();
    void canvicam2();
    void altura(int a);
  
  signals:
    void enviaAltura(int a);

  protected:
    virtual void paintGL ();
    virtual void initializeGL ( );
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void modelTransformPatricio ();
    virtual void modelTransformAvio ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void iniCamera2 ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void enviaPosFocus ();
    virtual void iniMaterialTerra ();
    virtual void carregaShaders ();

    //GLuint escalaAvioLoc, escalaPatriLoc;
    GLuint transAvioLoc;
    GLuint canviafocusLoc;
    float dist;
    glm::vec3 vrp;

    float canviafocus;

    float fov2,zn2,zf2;
    glm::vec3 vrp2,obs,up;

    float angleAvio;
    float alturaAvio;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
