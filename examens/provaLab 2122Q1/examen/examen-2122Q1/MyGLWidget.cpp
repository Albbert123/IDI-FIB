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

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);

  canviafocus = 0;
  angleAvio = 0.f;
  alturaAvio = 0.f;

  carregaShaders();
  iniEscena ();
  iniCamera ();
  iniCamera2 ();
}


void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersAvio();
  creaBuffersHangar();
  creaBuffersTerra();

  // Paràmetres de l'escena 
  glm::vec3 Pmin = glm::vec3(0,0,0);
  glm::vec3 Pmax = glm::vec3(30,2.5,24);

  centreEsc = (Pmin+Pmax) / 2.f;
  radiEsc = (glm::distance(Pmin,Pmax)) / 2;
  dist = 2*radiEsc;
}

void MyGLWidget::iniMaterialTerra()
{
  amb = glm::vec3(0.2,0.1,0.2);
  diff = glm::vec3(0,0.8,0.8);
  spec = glm::vec3(1,1,1);
  shin = 500;
}

void MyGLWidget::enviaPosFocus()
{
  if(canviafocus==0) posFoc = glm::vec3(0,0,0);
  else posFoc = glm::vec3((capsaAvioMax.x+capsaAvioMin.x) / 2, (capsaAvioMax.y+capsaAvioMin.y) / 2, capsaAvioMin.z); //capsa avio min
  glUniform3fv (posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera ()
{
  angleY = -0.5;
  angleX = 0.5;
  camera2 = false;
  ra = float(width())/height();
  fov = 2.f * glm::asin(radiEsc/dist); //float(M_PI/3.0);
  zn = dist - radiEsc; //15
  zf = dist + radiEsc; //65

  vrp = glm::vec3(centreEsc);

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::iniCamera2 ()
{
  ra = float(width())/height();
  fov2 = float(M_PI/2); 
  zn2 = 1; //15
  zf2 = zf; //65

  vrp2 = glm::vec3(15,0,12);
  obs = glm::vec3(15,3,12);
  up = glm::vec3(0,1,0);

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  ExamGLWidget::paintGL();
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3 (15, 0, 12));
  TG = glm::rotate(TG, float(-M_PI/2),glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escalaPat));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  //glUniform1f (escalaPatriLoc, 2*escalaPat);
}

void MyGLWidget::modelTransformAvio ()
{
  TGAvio = glm::mat4(1.f);
  
  TGAvio = glm::translate(TGAvio, glm::vec3 (15, alturaAvio, 12)); //lo que li queda
  TGAvio = glm::rotate(TGAvio, angleAvio, glm::vec3(0,1,0));
  TGAvio = glm::translate(TGAvio, glm::vec3 (10, 0, 0)); //distancia que esta del patri
  TGAvio = glm::scale(TGAvio, glm::vec3 (1.5*escalaAvio));
  TGAvio = glm::translate(TGAvio, -centreBaseAvio);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGAvio[0][0]);
  glUniformMatrix4fv (transAvioLoc, 1, GL_FALSE, &TGAvio[0][0]);
  
}

void MyGLWidget::viewTransform ()
{
  if (!camera2) 
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -dist));
    View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -vrp);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    View = glm::rotate(glm::mat4(1.f), float(-M_PI/2), glm::vec3(0, 1, 0));
    View = glm::translate(View, glm::vec3(0, -3, 0));
    View = glm::translate(View, -vrp); //centre escena es on esta el patri

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camera2)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov2, ra, zn2, zf2);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      ++alturaAvio;
      if(alturaAvio>5) alturaAvio = 5;
      emit enviaAltura(alturaAvio);
      modelTransformAvio();
    break;
	}
  case Qt::Key_Down: {
      --alturaAvio;
      if(alturaAvio<0) alturaAvio = 0;
      emit enviaAltura(alturaAvio);
      modelTransformAvio();
    break;
	}
  case Qt::Key_Right: {
      angleAvio -= (2*M_PI/32.f);
      modelTransformAvio();
    break;
	}
  case Qt::Key_F: {
      if(canviafocus == 1) canviafocus = 0;
      else canviafocus = 1;
      glUniform1f(canviafocusLoc, canviafocus);
      enviaPosFocus();
    break;
	}
  case Qt::Key_C: {
      camera2 = !camera2;
      viewTransform();
      projectTransform();
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


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camera2)
  {
    // Fem la rotació
    angleX += (e->y() - yClick) * M_PI / alt;
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::carregaShaders() {
  ExamGLWidget::carregaShaders();
  //escalaAvioLoc = glGetUniformLocation (program->programId(), "escAvio");
  //escalaPatriLoc = glGetUniformLocation (program->programId(), "escPatri");
  transAvioLoc = glGetUniformLocation (program->programId(), "TGAvio");
  canviafocusLoc = glGetUniformLocation (program->programId(), "canviAvio");
}

void MyGLWidget::canvicam1() {
  makeCurrent();

  camera2 = false;
  viewTransform();
  projectTransform();  

  update();
}

void MyGLWidget::canvicam2() {
  makeCurrent();

  camera2 = true;
  viewTransform();
  projectTransform();  

  update();
}

void MyGLWidget::altura(int a) {
  makeCurrent();

  alturaAvio = float(a);

  update();
}
