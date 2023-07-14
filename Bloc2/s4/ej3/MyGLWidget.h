// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
#include <QMouseEvent>
#include<QTimer>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    //void animacio ();
    void zoom(int z);
    void lego();
    void optica();


  protected:
    virtual void carregaShaders ();
    virtual void initializeGL ();
    virtual void paintGL (); 
    virtual void creaBuffers();
    virtual void ini_camera();

    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void modelTransformP ();
    virtual void modelTransformP1 ();
    virtual void modelTransformP2 ();
    //virtual void modelTransformP3 ();
    virtual void terraTransform ();
    void legomanTransform();

    virtual void creaBuffTerra();
    virtual void creaBuffersPatricio();
    virtual void creaBuffersLegoman();
    void caja_patricio();

    void caja_legoman();
    virtual void resizeGL (int w, int h);
    
    virtual void keyPressEvent(QKeyEvent* event);
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
    float theta = M_PI/12; //eix x (angle inicial perquè es comenci veient una mica inc)

    //mouse
    float old_x = 0.f;
    float old_y = 0.f;
    bool press = false;

    //patri
    GLuint VAO_Patricio;
    float nVerticesP;
    float angP = M_PI/2;
    float angP1 = 0.f;
    float angP2 = M_PI;
    //float angP3 = 0.f;
    glm::vec3 centrePatricio;
    Model m; //patricio
    float altura;


    //Terra
    GLuint VAO_Terra;

    QTimer timer;

    //Lego
    GLuint VAO_Legoman;
    float nVerticesL;
    Model l; //legoman
    float alturaL;
    glm::vec3 centreLego;


    //escena
    void cajaEscena();
    glm::vec3 centreEscena;
    float R;
    float d;
    

    float xmax,ymax,zmax;
    float xmin,ymin,zmin;

    bool prespectiva;

    bool modelPatricio;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
