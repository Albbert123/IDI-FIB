// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void carregaShaders ();
    virtual void initializeGL ();
    virtual void paintGL (); 
    virtual void creaBuffers();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void ini_camera();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransform ();
    virtual void terraTransform ();
    virtual void creaBuffTerra();
    void caja_homer();
    virtual void resizeGL (int w, int h);
    
    GLuint projLoc;
    GLuint viewLoc;

    //Atributs de camara
    float FOV;
    float FOVini;
    float raw;
    float zNear;
    float zFar;
    
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    //Homer
    GLuint VAO_Homer;
    float nVertices;
    float ang = 0.0f;

    //Terra
    GLuint VAO_Terra;

    Model m;
    glm::vec3 centre;
    float R;
    float d;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
