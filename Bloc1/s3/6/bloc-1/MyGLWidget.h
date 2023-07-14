#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"
#define GLM_FORCE_RADIANS

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    virtual void modelTransform1 ();

    virtual void modelTransform2 ();

    virtual void keyPressEvent ( QKeyEvent * e );

    virtual void mouseMoveEvent(QMouseEvent *e);

  private:
    void creaBuffers ();
    void carregaShaders ();
    
    // attribute locations
    GLuint vertexLoc;
    GLuint transLoc;
    GLuint colorLoc;
    GLuint valLoc;
    float tx = 0.f;
    float ty = 0.f;
    float angle = 0.f;
    float scale = 1.f, scalex=1.f, scaley=1.f;;
    float x_ant,y_ant;
    float rotate1=0.f, rotate2 = 0,f;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1,VAO2;
    GLint ample, alt;
};
