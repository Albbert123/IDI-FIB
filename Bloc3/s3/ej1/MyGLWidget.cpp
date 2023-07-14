// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_K: 
        posFocus.x-= 0.1;
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        break;
    
    case Qt::Key_L:
        posFocus.x += 0.1;
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        break;
    
    case Qt::Key_F:
        focusEsc = !focusEsc;
        glUniform1i(focusEscenaLoc, focusEsc);
        break;

      case Qt::Key_Right:
        posFocus.x += 0.1;
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        moux += 0.1;
        modelTransformPatricio();
        break;

        case Qt::Key_Left:
        posFocus.x-= 0.1;
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        moux -= 0.1;
        modelTransformPatricio();
        break;

        case Qt::Key_Up:
        posFocus.z += 0.1;
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        mouz += 0.1;
        modelTransformPatricio();
        break;

        case Qt::Key_Down:
        posFocus.z-= 0.1;
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        mouz -= 0.1;
        modelTransformPatricio();
        break;

        case Qt::Key_1:
            // color focus1
        apaga1 = !apaga1;
        if(apaga1) colorFocus = glm::vec3(0, 0, 0);
        else colorFocus = glm::vec3(0.8, 0.8, 0);
        glUniform3fv(colorFocusLoc, 1, &colorFocus[0]);
        break;

        case Qt::Key_2:
        // color focus2
        apaga2 = !apaga2;
        if(apaga2) colorFocus2 = glm::vec3(0, 0, 0);
        else colorFocus2 = glm::vec3(0.8, 0.8, 0.8);
        glUniform3fv(colorFocusLoc2, 1, &colorFocus2[0]);
        break;

        case Qt::Key_B:
        
        cul = !cul;
        if(cul) glEnable (GL_CULL_FACE);
        else glDisable (GL_CULL_FACE);
        break;

    default: BL3GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();

    posFocus.x = 0;
    posFocus.z = 0;
  inillum();

  iniEscena();
  iniCamera();
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.2,0,0.2);
  diff = glm::vec3(0,0,1);
  spec = glm::vec3(1,1,1);
  shin = 100;
}

void MyGLWidget::inillum ()
{
    // color focus1 escena
    colorFocus = glm::vec3(0.8, 0.8, 0);
    glUniform3fv(colorFocusLoc, 1, &colorFocus[0]);

    // color focus2 cameera
    colorFocus2 = glm::vec3(0.8, 0.8, 0.8);
    glUniform3fv(colorFocusLoc2, 1, &colorFocus2[0]);

    // posició del focus
    posFocus = glm::vec3(posFocus.x, 0.5, posFocus.z);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);

     // posició del focus2
    posFocus2 = glm::vec3(0,0,0);
    glUniform3fv(posFocusLoc2, 1, &posFocus2[0]);

    // llum ambient
    llumAmbient = glm::vec3(0.2, 0.2, 0.2);
    glUniform3fv(llumAmbientLoc, 1, &llumAmbient[0]);

    focusEsc = 1;
    glUniform1i(focusEscenaLoc, focusEsc);
}

void MyGLWidget::modelTransformPatricio ()
{

  TG = glm::translate(glm::mat4(1.f), glm::vec3(moux,0,mouz));
  TG = glm::translate(TG, glm::vec3(0,-(1-0.3/2),0));
  TG = glm::scale(TG, glm::vec3(escala*0.3/2, escala*0.3/2, escala*0.3/2));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escala = 2/(maxy-miny);
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;
}

void MyGLWidget::carregaShaders()
{
    BL3GLWidget::carregaShaders();
    colorFocusLoc = glGetUniformLocation (program->programId(), "colorFocus");
    colorFocusLoc2 = glGetUniformLocation (program->programId(), "colorFocus2");
    posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
    posFocusLoc2 = glGetUniformLocation (program->programId(), "posFocusO");
    llumAmbientLoc = glGetUniformLocation (program->programId(), "llumAmbient");
    focusEscenaLoc = glGetUniformLocation (program->programId(), "focusEsc");
}

