// MyGLWidget.cpp
#include "MyGLWidget.h"
#include "model.h"
#include <vector>
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

void MyGLWidget::initializeGL () {
    initializeOpenGLFunctions();  

    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST); // NOMES UN COP! En paintGL() molt incorrecte

    connect(&timer,SIGNAL (timeout()), this, SLOT (animacio()));
    timer.start(100); // s’activa cada 100 milisegons
    //timer.stop();  // atura animació

    prespectiva = true;

    carregaShaders();
    creaBuffers();

    escala = 1.0f;
    
    caja_patricio();
    
    ini_camera();
}

void MyGLWidget::animacio () {
    //glm::mat4 transform (1.0f);
    makeCurrent();
    
    angP3 -= M_PI/12.f;

    update();
}


void MyGLWidget::paintGL () 
{
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);
    
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Activem VAO patricio
    glBindVertexArray (VAO_Patricio);

    // Carreguem la transformació de model patricio
    modelTransformP (); 
    glDrawArrays(GL_TRIANGLES, 0, nVertices);

    // Carreguem la transformació de model patricio1
    modelTransformP1 (); 
    glDrawArrays(GL_TRIANGLES, 0, nVertices);

    // Carreguem la transformació de model patricio2
    modelTransformP2 (); 
    glDrawArrays(GL_TRIANGLES, 0, nVertices);

    // Carreguem la transformació de model patricio3
    modelTransformP3 (); 
    glDrawArrays(GL_TRIANGLES, 0, nVertices);

    terraTransform();
    glBindVertexArray (VAO_Terra);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::creaBuffTerra()
{
    glm::vec3 pos_terra[6] = 
    {
        glm::vec3(-2.5,0,2.5),
        glm::vec3(2.5,0,2.5),
        glm::vec3(-2.5,0,-2.5),

        glm::vec3(-2.5,0,-2.5),
        glm::vec3(2.5,0,2.5),
        glm::vec3(2.5,0,-2.5)
    };

    glm::vec3 col_terra[6] = 
    {
        glm::vec3(1,0,1),
        glm::vec3(1,0,1),
        glm::vec3(1,0,1),
        glm::vec3(1,0,1),
        glm::vec3(1,0,1),
        glm::vec3(1,0,1)
    };

    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    GLuint VBO_Terra[2];
    glGenBuffers(2, VBO_Terra);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_terra), pos_terra, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(col_terra), col_terra, GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::creaBuffers () 
{

    m.load ("/home/albert/Documentos/UPC/Q4/IDI/Bloc2/s3/ej123/bloc-2/Model/Patricio.obj");

    nVertices = m.faces().size()*3;

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    GLuint VBO_Patricio[2];
    glGenBuffers(2, VBO_Patricio);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nVertices*3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nVertices*3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    creaBuffTerra();

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders() {

    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::terraTransform () 
{
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformP () //patricio (0,0,0) X+
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, angP, glm::vec3(0.0, 1.0, 0.0));
  transform = glm::scale(transform, glm::vec3(escala)); 
  transform = glm::scale(transform, glm::vec3(1/altura, 1/altura, 1/altura)); //altura 1
  transform = glm::translate(transform, -centrePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformP1 () //patricio1 (2,0,2) Z+
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(2.f,0,2.f));
  transform = glm::rotate(transform, angP1, glm::vec3(0.0, 1.0, 0.0));
  transform = glm::scale(transform, glm::vec3(escala)); 
  transform = glm::scale(transform, glm::vec3(1/altura, 1/altura, 1/altura)); //altura 1
  transform = glm::translate(transform, -centrePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformP2 () //patricio2 (-2,0,-2) Z-
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-2.f,0,-2.f));
  transform = glm::rotate(transform, angP2, glm::vec3(0.0, 1.0, 0.0));
  transform = glm::scale(transform, glm::vec3(escala)); 
  transform = glm::scale(transform, glm::vec3(1/altura, 1/altura, 1/altura)); //altura 1
  transform = glm::translate(transform, -centrePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformP3 () //patricio3 (1.5,0,0) Z+
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, angP3, glm::vec3(0.0, 1.0, 0.0));
  transform = glm::translate(transform, glm::vec3(1.5f,0,0.f));
  transform = glm::scale(transform, glm::vec3(escala)); 
  transform = glm::scale(transform, glm::vec3(1/altura, 1/altura, 1/altura)); //altura 1
  transform = glm::translate(transform, -centrePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform () {

    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj;
    if(prespectiva) Proj = glm::perspective (FOV, raw, zNear, zFar);
    else Proj = glm::ortho(left, right, bottom, top, zNear, zFar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
    // glm::lookAt (OBS, VRP, UP)
    // glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glm::mat4 View (1.f);  
    View = glm::translate(View,glm::vec3(0,0,-d));
    View = glm::rotate(View,theta,glm::vec3(1.f,0.f,0.f)); //eix x, moviment dalt/abaix
    View = glm::rotate(View,-psi,glm::vec3(0.f,1.f,0.f)); //eix y, moviment esquerre/dreta
    View = glm::translate(View,-VRP);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::ini_camera(){

    //prespective
    FOVini = 2.f * glm::asin(R/d);
    FOV = FOVini;
    raw = 1.0f;
    zNear = d-R;
    zFar = d+R;

    //orto
    left = -R;
    right = R;
    bottom = -R;
    top = R;
    
    //view
    VRP = glm::vec3(centreEscena);
    OBS = VRP + glm::vec3(0.0, 0.0, d);
    UP = glm::vec3(0,1,0);

    //euler


    projectTransform();
    viewTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      escala += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      escala -= 0.05;
      break;
    }
    case Qt::Key_R: { // escalar a més gran
      angP += float(M_PI)/4.0f;
      angP1 += float(M_PI)/4.0f;
      angP2 += float(M_PI)/4.0f;
      break;
    }
    case Qt::Key_O: { // cambiar tipus camera
      prespectiva = !prespectiva;
      projectTransform();
      break;
    }
    case Qt::Key_X: { // zoom-out
    if(prespectiva) FOV += 0.1f;
    else {
        bottom -= 0.1;
        top += 0.1;
        left -= 0.1;
        right += 0.1;
    }

      projectTransform();
      break;
    }
    case Qt::Key_Z: { // cambiar tipus camera
    if(prespectiva) FOV -= 0.1f;
    else {
        bottom += 0.1;
        top -= 0.1;
        left += 0.1;
        right -= 0.1;
    }
      projectTransform();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void  MyGLWidget::mousePressEvent ( QMouseEvent * e ) {
    if ( e->buttons() == Qt::LeftButton ) press = true;
    old_x = e->x();
    old_y = e->y();
}
void  MyGLWidget::mouseReleaseEvent ( QMouseEvent * e ) {
    if ( e->buttons() == Qt::LeftButton ) press = false;
}
void  MyGLWidget::mouseMoveEvent ( QMouseEvent * e ) {
    if(press) {
        makeCurrent();
        
        psi += M_PI * (e->x() - old_x) / ample;//eix y, moviment esquerra/dreta (x)
        theta -= M_PI * (e->y() - old_y) / alt; //eix x, moviment baix/dalt (y)
        old_x = e->x();
        old_y = e->y();
        viewTransform();

        update();
    }
}

void MyGLWidget::caja_patricio() {
    m.load ("/home/albert/Documentos/UPC/Q4/IDI/Bloc2/s3/ej123/bloc-2/Model/Patricio.obj");

    xmax = xmin = float(m.vertices()[0]);
    ymax = ymin = float(m.vertices()[1]);
    zmax = zmin = float(m.vertices()[2]);

    for (unsigned int i = 0; i < m.vertices().size(); i+=3) {
        
        xmax = std::max(xmax,float(m.vertices()[i]));
        xmin = std::min(xmin,float(m.vertices()[i]));

        ymax = std::max(ymax,float(m.vertices()[i+1]));
        ymin = std::min(ymin,float(m.vertices()[i+1]));

        zmax = std::max(zmax,float(m.vertices()[i+2]));
        zmin = std::min(zmin,float(m.vertices()[i+2]));
    }

    altura = (ymax-ymin); //altura patricio

    centrePatricio = glm::vec3((xmax+xmin)/2, ymin, (zmax+zmin)/2);
    
    //calculem centre escena
    xmax = 2.5;
    xmin = -2.5;

    ymax = 1.f;
    ymin = 0.f;

    zmax = 2.5f;
    zmin = -2.5f;

    glm::vec3 Pmin = glm::vec3(xmin,ymin,zmin);
    glm::vec3 Pmax = glm::vec3(xmax,ymax,zmax);

    centreEscena = (Pmin+Pmax) / 2.f;
    R = (glm::distance(Pmin,Pmax)) / 2;
    d = 2*R;
}

void MyGLWidget::resizeGL (int w, int h) {

    ample = w;
    alt = h;

    float rav = float(w) / float(h);
    raw = rav;
    
    if(prespectiva) {
        if(rav<1.f) {
            FOV = 2.f * atan(tan(FOVini/2.f)/rav); //FOVini = 2 * anglev  -> anglev = FOVini/2
        }
    }

    else { //ortogonal
        if(rav > 1.f) {
            left = -rav * R;
            right = rav * R;
        }
        else {
            top = R / rav;
            bottom = -R / rav;
        }
    }
    projectTransform();
    //glViewport(0,0,w,h);
}


MyGLWidget::~MyGLWidget() {
}



