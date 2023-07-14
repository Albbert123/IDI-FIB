
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
  
  glClearColor (0.5, 0.5, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)

  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  glViewport (0, 0, ample/2, alt/2);
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 9);
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Activem l'Array a pintar 
  glBindVertexArray(VAO3);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);


  glViewport (ample/2, alt/2, ample/2, alt/2);
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 9);
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Activem l'Array a pintar 
  glBindVertexArray(VAO3);
 
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

void MyGLWidget::creaBuffers ()
{
  glm::vec3 Vertices[9];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.5, -0.5, 0.0); //punt esquerre baix
  Vertices[1] = glm::vec3(0.5, -0.5, 0.0); //punt dret baix
  Vertices[2] = glm::vec3(0.5, 0.5, 0.0); // punt dret dalt
  
  Vertices[3] = glm::vec3(-0.5, -0.5, 0.0); //punt esquerre baix
  Vertices[4] = glm::vec3(-0.5, 0.5, 0.0); //punt esquerre alt
  Vertices[5] = glm::vec3(0.5, 0.5, 0.0); // punt dret dalt

  Vertices[6] = glm::vec3(0.0, 1.0, 0.0); //punt esquerre baix
  Vertices[7] = glm::vec3(-0.5, 0.5, 0.0); //punt esquerre alt
  Vertices[8] = glm::vec3(0.5, 0.5, 0.0); // punt dret dalt
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); //pasa los datos de ram a la tarjeta grafica
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //estructura de buffer
  glEnableVertexAttribArray(vertexLoc);

  glm::vec3 Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(-1.0, 1.0, 0.0); //punt esquerre dalt
  Vertices2[1] = glm::vec3(0.5, -0.5, 0.0); //punt dret baix
  Vertices2[2] = glm::vec3(0.5, 0.5, 0.0); // punt dret dalt

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  //GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW); //pasa los datos de ram a la tarjeta grafica
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //estructura de buffer
  glEnableVertexAttribArray(vertexLoc);


  glm::vec3 Vertices3[3];  // Tres vèrtexs amb X, Y i Z
  Vertices3[0] = glm::vec3(1.0, -1.0, 0.0); //punt dret alt
  Vertices3[1] = glm::vec3(0.5, -0.5, 0.0); //punt dret baix
  Vertices3[2] = glm::vec3(0.5, 0.5, 0.0); // punt dret dalt

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO3);
  glBindVertexArray(VAO3);

  // Creació del buffer amb les dades dels vèrtexs
  //GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices3), Vertices3, GL_STATIC_DRAW); //pasa los datos de ram a la tarjeta grafica
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //estructura de buffer
  glEnableVertexAttribArray(vertexLoc);

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
}
