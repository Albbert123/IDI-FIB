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

void MyGLWidget::initializeGL ( ) {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  
  centreBaseCub = glm::vec3(0);
  angleP = 0.f;
  activaCub = true;
  focogroc = false;

  pos = 0;

  iniEscena ();
  iniCamera ();
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if(!activaCub) {
    // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else {
    posicions();
  }
  
  
  glBindVertexArray(0);
}

void MyGLWidget::iniCamera () {
  angleY = 0.65;
  angleX = -1.2;
  camPlanta = false;
  ra=float(width())/height();
  fov = float(M_PI/3.0);
  zn = radiEsc;
  zf = 3*radiEsc;

  //orto
  left = -radiEsc;
  right = radiEsc;
  bottom = -radiEsc;
  top = radiEsc;

  projectTransform ();
  viewTransform ();
}

void ExamGLWidget::enviaColFocus()
{
  glUniform3fv (colfocusLoc, 1, &colFoc[0]);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{

  TG = glm::mat4(1.f);
  TG = glm::rotate(TG,angle,glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3(escala));
  TG = glm::translate(TG, glm::vec3(-centreBaseCub));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);

  TG = glm::rotate(TG, angleP, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::rotate(TG, float(-M_PI/2), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(1, 0, 0)); //-anglex
    View = glm::rotate(View, float(M_PI/2), glm::vec3(0, 1, 0)); //angley
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  
    Proj = glm::ortho(left, right, bottom, top, zn, zf);
  
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      activaCub = !activaCub;
      paintGL();
    break;
	}
  case Qt::Key_1: {
      angleP = 0;
      emit setpos_patri(1);
      modelTransformPatricio();
    break;
	}
  case Qt::Key_2: {
      angleP = 2.f*M_PI/3.f;
      emit setpos_patri(2);
      modelTransformPatricio();
    break;
	}
  case Qt::Key_3: {
      angleP = -2.f*M_PI/3.f;
      emit setpos_patri(3);
      modelTransformPatricio();
    break;
	}
  case Qt::Key_F: {
      focogroc = !focogroc;
      if(focogroc) colFoc = glm::vec3(1,1,0);
      else colFoc = glm::vec3(1,1,1);
      enviaColFocus();
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      projectTransform();
      viewTransform();
    break;
	}
  case Qt::Key_Right: {
      ++pos;
      pos = pos%3;
      angleP += 2.f*M_PI/3.f;
      modelTransformPatricio();
      posicions();
    break;
	}
  case Qt::Key_Left: {
      --pos;
      pos = pos%3;
      angleP -= 2.f*M_PI/3.f;
      modelTransformPatricio();
      posicions();
    break;
	}
  case Qt::Key_R: {
      initializeGL();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::resizeGL (int w, int h) {
  #ifdef __APPLE__
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
  #else
    ample = w;
    alt = h;
  #endif

  ra = float(ample)/float(alt);

  if(camPlanta) { //orto
    if(ra > 1.f) {
      left = -ra * radiEsc;
      right = ra * radiEsc;
    }
    else {
      top = radiEsc / ra;
      bottom = -radiEsc / ra;
    }
  }
  projectTransform();
}

void MyGLWidget::posicions() {
    // Pintem el cub 1
    glBindVertexArray(VAO_Cub);
    modelTransformCub (2.0/0.5, pos*(2.0*M_PI/3));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pintem el cub 2
    modelTransformCub (2.5/0.5, (pos+1)*(2.0*M_PI/3));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pintem el cub 3
    modelTransformCub (3/0.5, (pos+2)*(2.0*M_PI/3));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MyGLWidget::optica() {
  makeCurrent();

  camPlanta = !camPlanta;
  projectTransform();
  viewTransform();

  update();
}

void MyGLWidget::pos_patri(int i) {
  makeCurrent();

  if(i == 1) {
    angleP = 0;
    modelTransformPatricio();
  }
  else if(i == 2) {
    angleP = 2.f*M_PI/3.f;
    modelTransformPatricio();
  }
  else {
    angleP = -2.f*M_PI/3.f;
    modelTransformPatricio();
  }

  update();
}