#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();
  
  public slots:
    void optica1();
    void optica2();
    void girpatri(int i);
  
  signals:
    void enviaCam1();
    void enviaCam2();
    void enviagir(int i);

  protected:
    virtual void initializeGL();
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void modelTransformPatricio ();
    virtual void enviaPosFocus ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void resizeGL(int w, int h);
    virtual void carregaShaders ();

    float dist;
    float fovini;

    bool canvifoco;
    GLuint canvifocoLoc;

    float left,right,bottom,top;
    
    int anglepatri;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
