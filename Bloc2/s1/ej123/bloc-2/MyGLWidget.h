// MyGLWidget.h
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  virtual void carregaShaders ();
  virtual void paintGL();
  virtual void projectTransform ();
  virtual void viewTransform ();
  virtual void ini_camera();

  GLuint projLoc;
  GLuint viewLoc;

  float FOV,ra,Zn,Zf;

  glm::vec3 OBS;
  glm::vec3 VRP;
  glm::vec3 UP;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
