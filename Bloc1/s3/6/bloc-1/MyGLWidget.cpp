
//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  glUniform1f(valLoc, scale);
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  modelTransform1();

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);

  modelTransform2 ();

  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent (QKeyEvent *e) {
  makeCurrent (); // fa actiu el nostre context d’OpenGL
  glm::mat4 TG (1.f);
  switch ( e->key() ) {
    case Qt::Key_S :
      scale += 0.1;
      //glUniform1f (valLoc, scale);
      break;
    case Qt::Key_D :
      scale -= 0.1;
      //glUniform1f (valLoc, scale);
      break;
    case Qt::Key_Left :
      tx-=0.5f;
      angle += float(M_PI/4);
      break;
    case Qt::Key_Right :
      tx+=0.5f;
      angle += float(M_PI/4);
      break;
      case Qt::Key_Up :
      ty+=0.5f;
      angle += float(M_PI/4);
      break;
    case Qt::Key_Down :
      ty-=0.5f;
      angle += float(M_PI/4);
      break;
    case Qt::Key_P :
      rotate1 += float(M_PI/6);
      rotate2 -= float(M_PI/6);
      break;
    default: e->ignore (); // propagar al pare
  }
  update (); // provoca que es torni a pintar l’escena
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
	makeCurrent();
	
	if(e->x() > x_ant) scalex += 0.01;
	else if(e->x() < x_ant) scalex -= 0.01;
	
	if(e->y() > y_ant) scaley += 0.01;
	else if(e->y() < y_ant) scaley -= 0.01;
	
	x_ant = e->x();
	y_ant = e->y();
	
	update();
}

void MyGLWidget::modelTransform1 () {
  glm::mat4 TG (1.f); // Matriu de transformació, inicialment identitat
  TG = glm::rotate (TG, angle, glm::vec3 (0.0f, 0.0f, 1.f));
  TG = glm::translate (TG, glm::vec3 (tx, ty, 0.0f));
  TG = glm::scale(TG,glm::vec3 (scalex,scaley,0.0));
  TG = glm::rotate (TG, rotate1, glm::vec3 (0.0f, 0.0f, 1.f));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  glUniform1f (valLoc, scale);
}

void MyGLWidget::modelTransform2 () {
  glm::mat4 TG (1.f); // Matriu de transformació, inicialment identitat
  TG = glm::rotate (TG, angle, glm::vec3 (0.0f, 0.0f, 1.f));
  TG = glm::translate (TG, glm::vec3 (tx, ty, 0.0f));
  TG = glm::scale(TG,glm::vec3 (scalex,scaley,0.0));
  TG = glm::rotate (TG, rotate2, glm::vec3 (0.0f, 0.0f, 1.f));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  glUniform1f (valLoc, scale);
}

void MyGLWidget::creaBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(0.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(-0.5, 1.0, 0.0);

  glm::vec3 Colores[3];  // Tres vèrtexs amb X, Y i Z
  Colores[0] = glm::vec3(1.,0., 0.0); 
  Colores[1] = glm::vec3(0., 1., 0.0); 
  Colores[2] = glm::vec3(0.0, 0.0, 1.0); 
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  GLuint VBO2;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colores), Colores, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  //triangulo 2

  glm::vec3 Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(0.0, -1.0, 0.0);
  Vertices2[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices2[2] = glm::vec3(0.5, 1.0, 0.0);

  glm::vec3 Colores2[3];  // Tres vèrtexs amb X, Y i Z
  Colores2[0] = glm::vec3(1.,0., 0.0); 
  Colores2[1] = glm::vec3(0., 1., 0.0); 
  Colores2[2] = glm::vec3(0.0, 0.0, 1.0); 
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colores2), Colores2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  transLoc = glGetUniformLocation (program->programId(), "TG");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  valLoc = glGetUniformLocation (program->programId(), "val");
}
