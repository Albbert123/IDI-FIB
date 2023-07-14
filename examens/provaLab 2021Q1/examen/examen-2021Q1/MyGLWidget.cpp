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

void MyGLWidget::initializeGL() {
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();

  veurepatri = true;
  anglepatri = 0.f;
  canvicolfoc = false;

  i = 0;

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

  if(veurepatri) {
     // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else {
    // Pintem el cub
    glBindVertexArray(VAO_Cub);
    posicions();
  }
   glBindVertexArray(0);
}

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersTerra();
  creaBuffersCub();

  // Paràmetres de l'escena -  ben calculats
  centreEsc = glm::vec3 (0, 1.5, 0);
  radiEsc = sqrt(10*10+10*10+1.5*1.5);

  colFoc = glm::vec3(1,1,1);
  enviaColFocus();
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.f;//0.65;
  angleX = 0.f;//-1.2;
  camPlanta = false;
  ra=float(width())/height();
  fov = float(M_PI/3.0);
  fovini = fov;
  zn = radiEsc;
  zf = 3*radiEsc;

  //cam2
  ra2 = ra;
  fov2 = 1.5*fov;
  zn2 = 0.5;
  zf2 = zf;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3(escala));
  TG = glm::translate(TG, -centreBaseCub);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, anglepatri, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::rotate(TG, float(-M_PI/2), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    glm::mat4 View(1.f);
    //View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    //View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    //View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
    View = glm::rotate(View, float(-M_PI/2), glm::vec3(0,1,0));
    View = glm::translate(View, glm::vec3(-5,0,0));
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
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov2,ra2,zn2,zf2);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      veurepatri = !veurepatri;
      paintGL();
    break;
	}
  case Qt::Key_1: {
      anglepatri = 0.f;
      emit enviapos(1);
      modelTransformPatricio();
    break;
	}
  case Qt::Key_2: {
      anglepatri = 2.f*M_PI/3;
      emit enviapos(2);
      modelTransformPatricio();
    break;
	}
  case Qt::Key_3: {
      anglepatri = -2.f*M_PI/3;
      emit enviapos(3);
      modelTransformPatricio();
    break;
	}
  case Qt::Key_F: {
      canvicolfoc = !canvicolfoc;
      if(!canvicolfoc) colFoc = glm::vec3(1,1,1);
      else colFoc = glm::vec3(1,1,0);
      enviaColFocus();
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      if(!camPlanta) emit enviacam1();
      else emit enviacam2();
      projectTransform();
      viewTransform();
    break;
	}
  case Qt::Key_Right: {
      ++i;
      i = i%3;
      anglepatri += 2.f*M_PI/3;
      posicions();
    break;
	}
  case Qt::Key_Left: {
      --i;
      i = i%3;
      anglepatri -= 2.f*M_PI/3;
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

void MyGLWidget::resizeGL(int w, int h) {
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
  if(!camPlanta) {
    if(ra<1.f) {
      fov = 2.f * atan(tan(fovini/2)/ra);
    }
  }
  
  projectTransform();
}

void MyGLWidget::posicions() {
    
    //cub1
    //modelTransformCub (2/0.5, (i)*2.f*M_PI/3);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //cub2
    modelTransformCub (2.5/0.5, (i+1)*2.f*M_PI/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //cub3
    modelTransformCub (3/0.5, (i+2)*2.f*M_PI/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}

void MyGLWidget::cam1() {
  makeCurrent();
  camPlanta = false;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::cam2() {
  makeCurrent();
  camPlanta = true;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::posp(int p) {
  makeCurrent();
  anglepatri = (p-1)*2.f*M_PI/3;
  modelTransformPatricio();
  update();
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camPlanta)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (yClick - e->y()) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}