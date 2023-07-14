#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffersModel();
  createBuffersTerra();

  amaga1 = amaga2 = amaga3 = amaga4 = false;
  camera2 = false;

  iniEscena();
  iniCamera();
  enviaColFocus();
}

void MyGLWidget::iniEscena ()
{
  glm::vec3 Pmin = glm::vec3(-10,0,-10);
  glm::vec3 Pmax = glm::vec3(10,8,10);
  centreEsc = (Pmin + Pmax) / 2.f;
  radiEsc = (glm::distance(Pmin,Pmax)) / 2.f;  
  dist = 2*radiEsc;
}

void MyGLWidget::iniCamera ()
{
    angleY = M_PI/4.0;
    angleX = 0;

    fov = 2.f * glm::asin(radiEsc / dist);
    fovini = fov;
    ra = float(width())/height();
    zn = dist - radiEsc;
    zf = dist + radiEsc;

    //cam2
    fov2 = M_PI/2.f;
    fovini2 = fov;
    ra2 = ra;
    zn2 = 1;
    zf2 = zf;

    projectTransform ();
    viewTransform ();
  
}


void MyGLWidget::enviaColFocus()
{
  colFoc = glm::vec3(1,1,1);
  glUniform3fv (colfocusLoc, 1, &colFoc[0]);
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);
  modelTransformIdent ();
  // pintem terra
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (VAO_Model);
  if(!amaga1) {
    // Activem el VAO per a pintar el pat1
    modelTransformModel1 ();
    // Pintem Model
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  }
  if(!amaga2) {
    // Activem el VAO per a pintar el pat2
    //glBindVertexArray (VAO_Model);
    modelTransformModel2 ();
    // Pintem Model
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  }
  if(!amaga3) {
    // Activem el VAO per a pintar el pat3
    //glBindVertexArray (VAO_Model);
    modelTransformModel3 ();
    // Pintem Model
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  }
  if(!amaga4) {
    // Activem el VAO per a pintar el pat4
    //glBindVertexArray (VAO_Model);
    modelTransformModel4 ();
    // Pintem Model
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  } 
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  alt = h;
  ample = w;

  ra = float(ample) / float(alt);

  if(ra < 1.f) {
    fov = 2.f * atan(tan(fovini/2.f)/ra);
  }

  projectTransform();
}

void MyGLWidget::modelTransformModel1 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(8,0,8));
  TG = glm::rotate(TG, float(-3*M_PI/4), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(2*escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(-8,0,8));
  TG = glm::rotate(TG, float(3*M_PI/4), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(4*escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel3 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(8,0,-8));
  TG = glm::rotate(TG, float(-M_PI/4), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(6*escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel4 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(-8,0,-8));
  TG = glm::rotate(TG, float(M_PI/4), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(8*escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformIdent ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  if(!camera2) {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov, ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  else if(camera2 and (amaga1 or amaga2 or amaga3 or amaga4)){
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov2, ra2, zn2, zf2);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  
}

void MyGLWidget::viewTransform ()
{
  if(!camera2) {

    glm::mat4 View;  // Matriu de posició i orientació
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else if(camera2 and (amaga1 or amaga2 or amaga3 or amaga4)){
   
    if(amaga1) {
      
      glm::mat4 View(1.f);  // Matriu de posició i orientació
      View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
      View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
      View = glm::translate(View, glm::vec3(-8,-2,-8));
      //View = glm::translate(View, -centreEsc); //(0,-4,0)

      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
    if(amaga2) {
      glm::mat4 View(1.f);  // Matriu de posició i orientació
      View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
      View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
      View = glm::rotate(View, float(M_PI/2), glm::vec3(0, 1, 0));
      View = glm::translate(View, glm::vec3(8,-4,-8));
      //View = glm::translate(View, -centreEsc); //(0,-4,0)

      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
    if(amaga3) {
      glm::mat4 View(1.f);  // Matriu de posició i orientació
      View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
      View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
      View = glm::rotate(View, float(-M_PI/2), glm::vec3(0, 1, 0));
      View = glm::translate(View, glm::vec3(-8,-6,8));
      //View = glm::translate(View, -centreEsc); //(0,-4,0)

      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
    if(amaga4) {
      glm::mat4 View(1.f);  // Matriu de posició i orientació
      View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
      View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
      View = glm::rotate(View, float(M_PI), glm::vec3(0, 1, 0));
      View = glm::translate(View, glm::vec3(8,-8,8));
      //View = glm::translate(View, -centreEsc); //(0,-4,0)

      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
  }
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::createBuffersModel ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  m.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel (m, escalaModel, centreBaseModel);
  
  // Creació del Vertex Array Object del Model
  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);

  // Creació dels buffers del model patr
  GLuint VBO_Model[6];
  // Buffer de posicions
  glGenBuffers(6, VBO_Model);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3, m.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::createBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[6] = {
        glm::vec3(-10.0, 0.0, -10.0),
        glm::vec3(-10.0, 0.0,  10.0),
        glm::vec3( 10.0, 0.0, -10.0),
        glm::vec3( 10.0, 0.0, -10.0),
        glm::vec3(-10.0, 0.0,  10.0),
        glm::vec3( 10.0, 0.0,  10.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 normt (0,1,0);
  glm::vec3 normterra[6] = {
	normt, normt, normt, normt, normt, normt
  };

  // Definim el material del terra
  glm::vec3 amb(0.2,0.2,0.2);
  glm::vec3 diff(0,0.9,0);
  glm::vec3 spec(0.6,0.6,0.6);
  float shin = 500;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[6] = {
	amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[6] = {
	diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[6] = {
	spec, spec, spec, spec, spec, spec
  };
  float matshinterra[6] = {
	shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[6];
  glGenBuffers(6, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
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
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  colfocusLoc = glGetUniformLocation (program->programId(), "colFocus");
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_H: { // help de l'aplicació
      std::cout << std::endl << "Ajuda: Tecles actives a l'aplicació" << std::endl;
      std::cout << "   H: Mostra aquest mateix help." << std::endl;
      break;
    }
    case Qt::Key_R: { 
      initializeGL();
      break;
    }
    case Qt::Key_C: { 
      camera2 = !camera2;
      projectTransform();
      viewTransform();
      break;
    }
    case Qt::Key_0: { 
      amaga1 = amaga2 = amaga3 = amaga4 = false; 
      emit envianingun();
      paintGL();
      break;
    }
    case Qt::Key_1: { 
      amaga1 = true;
      amaga2 = amaga3 = amaga4 = false;
      emit envia1();
      paintGL();
      break;
    }
    case Qt::Key_2: { 
      amaga2 = true;
      amaga1 = amaga3 = amaga4 = false;
      emit envia2();
      paintGL();
      break;
    }
    case Qt::Key_3: { 
      amaga3 = true;
      amaga1 = amaga2 = amaga4 = false;
      emit envia3();
      paintGL();
      break;
    }
    case Qt::Key_4: { 
      amaga4 = true;
      amaga1 = amaga2 = amaga3 = false; 
      emit envia4();
      paintGL();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
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

void MyGLWidget::amagapat1() {
  makeCurrent();
  amaga1 = true;
  amaga2 = amaga3 = amaga4 = false;
  paintGL();
  update();
}
void MyGLWidget::amagapat2() {
  makeCurrent();
  amaga2 = true;
  amaga1 = amaga3 = amaga4 = false;
  paintGL();
  update();
}
void MyGLWidget::amagapat3() {
  makeCurrent();
  amaga3 = true;
  amaga1 = amaga2 = amaga4 = false;
  paintGL();
  update();
}
void MyGLWidget::amagapat4() {
  makeCurrent();
  amaga4 = true;
  amaga1 = amaga2 = amaga3 = false;
  paintGL();
  update();
}
void MyGLWidget::ningun() {
  makeCurrent();
  amaga1 = amaga2 = amaga3 = amaga4 = false;
  paintGL();
  update();
}

