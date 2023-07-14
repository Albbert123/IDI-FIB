#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();
  
  public slots:
    void cam1();
    void cam2();
    void posp(int p);

  signals:
    void enviacam1();
    void enviacam2();
    void enviapos(int p);

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void initializeGL();
    virtual void paintGL ();
    virtual void iniEscena();
    virtual void iniCamera();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void resizeGL(int w, int h);
    void posicions();

    glm::vec3 centreBaseCub = glm::vec3(0.f);

    bool veurepatri;
    float anglepatri;

    bool canvicolfoc;

    float ra2,fov2,zn2,zf2;
    float fovini;

    int i;
  private:
    int printOglError(const char file[], int line, const char func[]);
};
