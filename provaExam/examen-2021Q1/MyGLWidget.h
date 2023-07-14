#include "ExamGLWidget.h"
#include <QTimer>

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void optica();
    void pos_patri(int i);
  
  signals:
    void setpos_patri(int i);

  protected:
    virtual void paintGL ();
    virtual void initializeGL ( );
    virtual void resizeGL (int width, int height);
    //virtual void enviaColFocus ();
    //virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    void posicions();

    glm::vec3 centreBaseCub;

    float angleP;
    bool activaCub;
    bool focogroc;

    //ortogonal
    float left;
    float right;
    float top;
    float bottom;

    int pos;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
