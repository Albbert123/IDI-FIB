#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  public slots:
    void amagapat1();
    void amagapat2();
    void amagapat3();
    void amagapat4();
    void ningun();
  
  signals:
    void envia1();
    void envia2();
    void envia3();
    void envia4();
    void envianingun();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffersModel ();
    void createBuffersTerra ();
    void carregaShaders ();
    void iniEscena ();
    void iniCamera ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformIdent ();
    void modelTransformModel1 ();
    void modelTransformModel2 ();
    void modelTransformModel3 ();
    void modelTransformModel4 ();
    void enviaColFocus();
    void calculaCapsaModel (Model &p, float &escala, glm::vec3 &CentreBase);

    // VAO i VBO names
    GLuint VAO_Model;
    GLuint VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    GLint alt, ample;

    GLuint colfocusLoc;
    glm::vec3 colFoc;
    // model
    Model m;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centreBaseModel;
    float escalaModel;
    // radi de l'escena
    float radiEsc;
    glm::vec3 centreEsc;
    float dist;

    float fov,fovini,ra,zn,zf;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY, angleX;

    bool amaga1,amaga2,amaga3,amaga4;

    bool camera2;
    float fov2,fovini2,ra2,zn2,zf2;

    float angleYaux, angleXaux;
};

