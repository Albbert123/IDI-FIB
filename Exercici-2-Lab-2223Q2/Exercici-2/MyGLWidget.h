#include "LL2GLWidget.h"

#include "model.h"
#include<QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void animacio ();
    //connect(&timer, SIGNAL(timeout()), this, SLOT (animacio()));

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
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event corresponent de ratolí
    // virtual void mousePressEvent (QMouseEvent *event);
    // virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    // iniEscena - Aqui incluim les inicialitzacions de l'escena
    virtual void iniEscena ();
    // iniCamera - Aqui incluim les inicialitzacions de la camera
    virtual void iniCamera ();    
    // creaBuffersModels - Aquí carreguem els fitxers obj i fem la inicialització dels diferents VAOS i VBOs
    //virtual void creaBuffersModels ();
    // creaBuffersTerra - Aqui inicialitzem el VAO i els VBO d'un model fet a mà que representa un terra
    // virtual void creaBuffersTerra ();
    // carregaShaders - Aquí carreguem els shaders, els compilem i els linkem. També busquem els uniform locations que fem servir.
    virtual void carregaShaders ();
    
    // viewTransform i projecTransform - Es fan servir per a construir i enviar als shader les matrius de càmera (View i Projection respectivament).
    virtual void viewTransform ();
    virtual void projectTransform ();
    
    // {Rick/Morty/VideoCamera/ident}Transform - Funciones per construir i enviar al shader diferents matrius de transformació geomètrica TG
    virtual void RickTransform();
    virtual void MortyTransform();
    virtual void VideoCameraTransform();
    //virtual void identTransform();
    //virtual void ParetTransform();

    glm::vec3 Pmin, Pmax;

    float psi;
    float theta;
    float d;
    float fovini;

    float angcamY;

    void ini_camera2();
    void viewTransform2();
    void projectTransform2();

    float fov2, ra2, znear2, zfar2;
    glm::vec3 obs2, vrp2, up2;

    bool segonacam;
    glm::vec3 colorcam2;
    GLuint color2Loc,pintaLoc;

    bool pintarick=false;
    float pinta=0;

    QTimer timer;
    bool activatimer;
    bool canvi;

  private:

  
    int printOglError(const char file[], int line, const char func[]);
    
};
