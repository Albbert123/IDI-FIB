// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void animacio();
    
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    
    virtual void initializeGL();
    virtual void inillum();
    virtual void paintGL();
    virtual void viewTransform ();
    virtual void modelTransformDiscoBall ();
    virtual void modelTransformMorty ();
    virtual void modelTransformFantasma (float posX);
    virtual void modelTransformFantasma2 (float posX);
    virtual void carregaShaders();
    virtual void estatsllum(int i);

    float angleX = 0.f;
    float girBola = 0.f;
    float girMorty = 0.f;
    float girFantasma = 0.f;

    glm::vec3 posFocusEsc, colorFocusEsc;
    glm::vec3 posFocus1, colorFocus1, posFocus2, colorFocus2, posFocus3, colorFocus3, posFocus4, colorFocus4;
    glm::vec3 llumAmbient;

    GLuint posFocusLoc;
    GLuint posFocusLoc1;
    GLuint posFocusLoc2;
    GLuint posFocusLoc3;
    GLuint posFocusLoc4;

    GLuint colorFocusLoc;
    GLuint colorFocusLoc1;
    GLuint colorFocusLoc2;
    GLuint colorFocusLoc3;
    GLuint colorFocusLoc4;

    GLuint llumAmbientLoc;

    GLuint bolatransLoc;

    bool apagaEsc = false;
    QString estat = "estat1";
    bool apagat1 = true;
    bool apagat2 = true;
    bool apagat3 = true;
    bool apagat4 = true;

    QTimer timer;
    bool activatimer = false;


  private:
    int printOglError(const char file[], int line, const char func[]);
};
