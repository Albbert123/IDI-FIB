// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  virtual void carregaShaders ();
  virtual void creaBuffersTerra ();
  virtual void creaBuffers ();
  virtual void paintGL();
  virtual void projectTransform ();
  virtual void viewTransform ();
  virtual void modelTransform ();
  virtual void modelTransformTerra ();
  virtual void ini_camera();
  virtual void keyPressEvent (QKeyEvent *event);

  GLuint projLoc;
  GLuint viewLoc;

  float FOV,ra,Zn,Zf;

  glm::vec3 OBS;
  glm::vec3 VRP;
  glm::vec3 UP;

  GLuint VAO_Hommer;
  GLuint VAO_Terra;
  float nvertices;
  float angle=0.0f;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
