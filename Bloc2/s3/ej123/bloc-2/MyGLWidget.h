// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
#include <QMouseEvent>

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
    void caja_patricio();
    virtual void resizeGL (int w, int h);

    virtual void mousePressEvent ( QMouseEvent * e );
    virtual void mouseReleaseEvent ( QMouseEvent * e );
    virtual void mouseMoveEvent ( QMouseEvent * e );
    
    GLuint projLoc;
    GLuint viewLoc;

    //Atributs de camara prepsective
    float FOV;
    float FOVini;
    float raw;
    float zNear;
    float zFar;

    //ortogonal
    float left;
    float right;
    float top;
    float bottom;
    
    //camera
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    //euler
    float psi = 0.0f; //eix y
    float theta = 0.0f; //eix x

    //mouse
    float old_x = 0.f;
    float old_y = 0.f;
    bool press = false;

    //patri
    GLuint VAO_Patricio;
    float nVertices;
    float ang = 0.0f;

    //Terra
    GLuint VAO_Terra;

    Model m;
    glm::vec3 centreEscena;
    glm::vec3 centrePatricio;
    float R;
    float d;
    float altura;



    float xmax,ymax,zmax;
    float xmin,ymin,zmin;

    bool prespectiva;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
