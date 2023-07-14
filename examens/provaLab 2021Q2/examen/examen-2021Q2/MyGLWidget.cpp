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

void MyGLWidget::initializeGL()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();

  canvifoco = false;
  anglepatri = 0.f;

  iniEscena ();
  iniCamera ();
}

void MyGLWidget::paintGL() 
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

  // Pintem el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pintem l'arbre1
  pintaArbre (glm::vec3(10,0,10));

  // Pintem l'arbre2
  pintaArbre (glm::vec3(20,0,5));

  // Pintem l'arbre3
  pintaArbre (glm::vec3(25,0,15));

  glBindVertexArray(0);
}


void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersTerra();
  creaBuffersCub();
  creaBuffersEsfera();

  glm::vec3 Pmin = glm::vec3(0,0,0);
  glm::vec3 Pmax = glm::vec3(30,4,20);

  // Paràmetres de l'escena
  centreEsc = (Pmin+Pmax) / 2.f;
  radiEsc = (glm::distance(Pmin,Pmax)) / 2;
  dist = 2*radiEsc;

  enviaPosFocus();
}

void MyGLWidget::enviaPosFocus()
{
  if(!canvifoco) posFoc = glm::vec3(0,0,0);
  else posFoc = glm::vec3(15,10,10);
  glUniform3fv (posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.5;
  angleX = -0.5;
  camPlanta = false;
  ra = float(width())/height();
  fov = 2.f * glm::asin(radiEsc/dist);
  fovini = fov;
  zn = dist - radiEsc;
  zf = dist + radiEsc;

  //orto
  left = -radiEsc;
  right = radiEsc;
  bottom = -radiEsc;
  top = radiEsc;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::resizeGL (int w, int h) 
{
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
      fov = 2.f * atan(tan(fovini/2.f)/ra); //FOVini = 2 * anglev  -> anglev = FOVini/2
    }
  }
  else { //ortogonal
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

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(10,0,10)); //el q ens queda per posarlo a on el inici
  TG = glm::rotate(TG, -float(float(anglepatri)*(2.f*M_PI/32.f)), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0)); //radi del q volem girar
  TG = glm::scale(TG, glm::vec3 (2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camPlanta) 
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, glm::vec3(-centreEsc));


    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
    
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(1, 0, 0));
    View = glm::rotate(View, 0.f, glm::vec3(0, 1, 0));
    View = glm::translate(View, glm::vec3(-centreEsc));


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
    Proj = glm::ortho(left, right, bottom, top, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      ++anglepatri;
      anglepatri = anglepatri % 32;
      emit enviagir(int(anglepatri)); 
      //modelTransformPatricio();
    break;
	}
  case Qt::Key_F: {
      canvifoco = !canvifoco;
      glUniform1f(canvifocoLoc, canvifoco);
      enviaPosFocus();
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      if (!camPlanta) emit enviaCam1();
      else emit enviaCam2();
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
  if ((DoingInteractive == ROTATE) && !camPlanta)
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
  canvifocoLoc = glGetUniformLocation (program->programId(), "canvifoco");
}

void MyGLWidget::optica1() {
  makeCurrent();
  camPlanta = false;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::optica2() {
  makeCurrent();
  camPlanta = true;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::girpatri(int i) {
  makeCurrent();

  anglepatri = float(i);

  update();
}


