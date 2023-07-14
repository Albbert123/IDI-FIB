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

    carregaShaders();
    creaBuffers();

    escala = 1.0f;
    
    caja_homer();
    
    ini_camera();
}

void MyGLWidget::paintGL () 
{
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);
    
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    modelTransform (); 
    glBindVertexArray (VAO_Homer);
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
        glm::vec3(-2,-1,2),
        glm::vec3(2,-1,2),
        glm::vec3(-2,-1,-2),

        glm::vec3(-2,-1,-2),
        glm::vec3(2,-1,2),
        glm::vec3(2,-1,-2)
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

    m.load ("/home/albert/Documentos/UPC/Q4/IDI/Bloc2/s2/ex123/bloc-2/Model/HomerProves.obj");

    nVertices = m.faces().size()*3;

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nVertices*3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
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

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(escala));
  transform = glm::rotate(transform,ang, glm::vec3(0,1,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform () {

    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective (FOV, raw, zNear, zFar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
    // glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::ini_camera(){

    FOVini = 2.f * glm::asin(R/d);
    FOV = FOVini;
    raw = 1.0f;
    zNear = d-R;
    zFar = d+R;
    
    //OBS = glm::vec3(0,0,d);
    VRP = glm::vec3(centre);
    OBS = VRP + d*glm::vec3(0.0, 0.0, 1.0);
    UP = glm::vec3(0,1,0);

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
      ang += float(M_PI)/4.0f;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::caja_homer() {
    m.load ("/home/albert/Documentos/UPC/Q4/IDI/Bloc2/s2/ex123/bloc-2/Model/HomerProves.obj");

    float xmax,ymax,zmax;
    float xmin,ymin,zmin;

    xmax = xmin = float(m.vertices()[0]);
    ymax = ymin = float(m.vertices()[1]);
    zmax = zmin = float(m.vertices()[2]);

    for (unsigned int i = 0; i < m.vertices().size(); i+=3) {
        // escric per pantalla les coordenades del vèrtex
        //std::cout << “(x, y, z) = (“ << m.vertices()[i] << “, “ << m.vertices()[i+1] << “, “ << m.vertices()[i+2] << “)” << std::endl;
        xmax = std::max(xmax,float(m.vertices()[i]));
        xmin = std::min(xmin,float(m.vertices()[i]));

        ymax = std::max(ymax,float(m.vertices()[i+1]));
        ymin = std::min(ymin,float(m.vertices()[i+1]));

        zmax = std::max(zmax,float(m.vertices()[i+2]));
        zmin = std::min(zmin,float(m.vertices()[i+2]));
    }

    //comparem amb el terra
    xmax = std::max(xmax,2.f);
    xmin = std::min(xmin,-2.f);

    ymax = std::max(ymax,1.f);
    ymin = std::min(ymin,-1.f);

    zmax = std::max(zmax,2.f);
    zmin = std::min(zmin,-2.f);

    glm::vec3 Pmin = glm::vec3(xmin,ymin,zmin);
    glm::vec3 Pmax = glm::vec3(xmax,ymax,zmax);

    centre = glm::vec3((xmax+xmin)/2, (ymax+ymin)/2, (zmax+zmin)/2);
    R = (glm::distance(Pmin,Pmax)) / 2;
    d = 2*R;
}

void MyGLWidget::resizeGL (int w, int h) {
    float rav = float(w) / float(h);
    raw = rav;

    if(rav<1.f) {
        FOV = 2.f * atan(tan(FOVini/2.f)/rav); //FOVini = 2 * anglev  -> anglev = FOVini/2
    }

    projectTransform();
    glViewport(0,0,w,h);

}


MyGLWidget::~MyGLWidget() {
}


/*
modelTransform() {
    scale(2/h.2/h.2/h) --> si queremos altura 2
    transalte(TG,-centreEsfera)
}
*/


