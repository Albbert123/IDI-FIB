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
  case Qt::Key_A: {
      girMorty -= M_PI / 4; //-45º
      girFantasma -= M_PI / 4; 
    break;
	}
  case Qt::Key_D: {
      girMorty += M_PI / 4; //45º
      girFantasma += M_PI / 4; 
    break;
	}
  case Qt::Key_E: {
    apagaEsc = !apagaEsc;
    if(apagaEsc) colorFocusEsc = glm::vec3(0, 0, 0);
    else colorFocusEsc = glm::vec3(0.4, 0.4, 0.4);
    glUniform3fv(colorFocusLoc, 1, &colorFocusEsc[0]);
    break;
	}
  case Qt::Key_B: {
      if(estat == "estat1") estatsllum(1);
      else if(estat == "estat2") estatsllum(2);
      else if(estat == "estat3") estatsllum(3);
      else if(estat == "estat4") estatsllum(4);
      glUniform3fv(colorFocusLoc1, 1, &colorFocus1[0]);
      glUniform3fv(colorFocusLoc2, 1, &colorFocus2[0]);
      glUniform3fv(colorFocusLoc3, 1, &colorFocus3[0]);
      glUniform3fv(colorFocusLoc4, 1, &colorFocus4[0]);
     break;
	}	
  case Qt::Key_R: {
      girBola += M_PI / 36; //5º
      modelTransformDiscoBall();
    break;
	}
  case Qt::Key_S: {
      activatimer = !activatimer;
      if(activatimer) timer.start(100);
      else timer.stop();
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  connect(&timer,SIGNAL (timeout()), this, SLOT (animacio()));
  
  carregaShaders();
  
  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersDiscoBall();
  creaBuffersTerraIParets();

  iniEscena();
  iniCamera();

  inillum();
}

void MyGLWidget::inillum() {

  // color focus escena
  colorFocusEsc = glm::vec3(0.4, 0.4, 0.4);
  glUniform3fv(colorFocusLoc, 1, &colorFocusEsc[0]);

  // color focus1 model
  colorFocus1 = glm::vec3(0.4, 0, 0);
  glUniform3fv(colorFocusLoc1, 1, &colorFocus1[0]);

  // color focus2 model
  colorFocus2 = glm::vec3(0, 0.4, 0);
  glUniform3fv(colorFocusLoc2, 1, &colorFocus2[0]);

  // color focus3 model
  colorFocus3 = glm::vec3(0, 0, 0.4);
  glUniform3fv(colorFocusLoc3, 1, &colorFocus3[0]);

  // color focus4 model
  colorFocus4 = glm::vec3(0.4, 0.4, 0);
  glUniform3fv(colorFocusLoc4, 1, &colorFocus4[0]);

  // posició del focus escena
  posFocusEsc = glm::vec3(5, 10, 5);
  glUniform3fv(posFocusLoc, 1, &posFocusEsc[0]);

  // posició del focus1
  posFocus1 = glm::vec3(1.2,0,0);
  glUniform3fv(posFocusLoc1, 1, &posFocus1[0]);

  // posició del focus2
  posFocus2 = glm::vec3(-1.2,0,0);
  glUniform3fv(posFocusLoc2, 1, &posFocus2[0]);

  // posició del focus3
  posFocus3 = glm::vec3(0,0,1.2);
  glUniform3fv(posFocusLoc3, 1, &posFocus3[0]);

  // posició del focus4
  posFocus4 = glm::vec3(0,0,-1.2);
  glUniform3fv(posFocusLoc4, 1, &posFocus4[0]);

  // llum ambient
  //llumAmbient = glm::vec3(0.1, 0.1, 0.1);
  //glUniform3fv(llumAmbientLoc, 1, &llumAmbient[0]);

}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA 1
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // FANTASMA 2
  //glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma2 (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::modelTransformDiscoBall () {
  glm::mat4 TG;
  
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::rotate(TG, girBola, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  glUniformMatrix4fv (bolatransLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMorty () {
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::rotate(TG, girMorty, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX) {
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::rotate(TG, girFantasma, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma2 (float posX) {
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::rotate(TG, girFantasma, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::carregaShaders()
{
  LL4GLWidget::carregaShaders();
  colorFocusLoc = glGetUniformLocation (program->programId(), "colorFocus");
  colorFocusLoc1 = glGetUniformLocation (program->programId(), "colorFocus1");
  colorFocusLoc2 = glGetUniformLocation (program->programId(), "colorFocus2");
  colorFocusLoc3 = glGetUniformLocation (program->programId(), "colorFocus3");
  colorFocusLoc4 = glGetUniformLocation (program->programId(), "colorFocus4");

  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
  posFocusLoc1 = glGetUniformLocation (program->programId(), "posFocus1");
  posFocusLoc2 = glGetUniformLocation (program->programId(), "posFocus2");
  posFocusLoc3 = glGetUniformLocation (program->programId(), "posFocus3");
  posFocusLoc4 = glGetUniformLocation (program->programId(), "posFocus4");

  //llumAmbientLoc = glGetUniformLocation (program->programId(), "llumAmbient");

  bolatransLoc = glGetUniformLocation (program->programId(), "TGbola");
}

void MyGLWidget::animacio() {
  makeCurrent();

  girBola += M_PI / 36; //5º
  modelTransformDiscoBall();

  if(estat == "estat1") estatsllum(1);
  else if(estat == "estat2") estatsllum(2);
  else if(estat == "estat3") estatsllum(3);
  else if(estat == "estat4") estatsllum(4);
  glUniform3fv(colorFocusLoc1, 1, &colorFocus1[0]);
  glUniform3fv(colorFocusLoc2, 1, &colorFocus2[0]);
  glUniform3fv(colorFocusLoc3, 1, &colorFocus3[0]);
  glUniform3fv(colorFocusLoc4, 1, &colorFocus4[0]);

  update();
}

void MyGLWidget::estatsllum(int i) {
  if(i == 1) {
    if(apagat1) colorFocus1 = glm::vec3(0, 0, 0);
    else colorFocus1 = glm::vec3(0.4, 0, 0);
    apagat1 = !apagat1;
    estat = "estat2";
  }
  else if(i == 2) {
    if(apagat2) colorFocus2 = glm::vec3(0, 0, 0);
    else colorFocus2 = glm::vec3(0, 0.4, 0);
    apagat2 = !apagat2;
    estat = "estat3";
  } 
  else if(i == 3) {
    if(apagat3) colorFocus3 = glm::vec3(0, 0, 0);
    else colorFocus3 = glm::vec3(0, 0, 0.4);
    apagat3 = !apagat3;
    estat = "estat4";
  }
  else if(i == 4) {
    if(apagat4) colorFocus4 = glm::vec3(0, 0, 0);
    else colorFocus4 = glm::vec3(0.4, 0.4, 0);
    apagat4 = !apagat4;
    estat = "estat1";
  }
}
