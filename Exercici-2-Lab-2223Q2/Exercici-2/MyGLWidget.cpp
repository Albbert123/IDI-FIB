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

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL ( ) {
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();  
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

    connect(&timer,SIGNAL (timeout()), this, SLOT (animacio()));

    carregaShaders();
    creaBuffersModels();
    creaBuffersTerra();
    iniEscena();
    iniCamera();
}

void MyGLWidget::animacio () {

    makeCurrent();

    if(angcamY <= M_PI/2 && !canvi) {
        angcamY += M_PI/12;
        if(angcamY >= M_PI/2) {
            angcamY = M_PI/2;
            canvi = !canvi;
        }
    }
    else if (angcamY >= 0 && canvi) {
        angcamY -= M_PI/12;
        if(angcamY <= 0) {
            angcamY = 0;
            canvi = !canvi;
        }
    }

    update();
}


void MyGLWidget::paintGL ( ) {

    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport (0, 0, ample, alt);
    iniCamera();

    colorcam2 = glm::vec3(1.0, 1.0, 1.0);
    glUniform3fv (color2Loc, 1, &colorcam2[0]);

    if(pintarick) {
        pinta = 1;
        glUniform1f(pintaLoc, pinta);
    }

    // Rick
    glBindVertexArray (VAO_models[RICK]);
    RickTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

    if(pintarick) {
        pinta = 0;
        glUniform1f(pintaLoc, pinta);
    }

    // Morty
    glBindVertexArray (VAO_models[MORTY]);
    MortyTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
    
    // VideoCamera
    glBindVertexArray (VAO_models[VIDEOCAMERA]);
    VideoCameraTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);  
    
    // Terra
    glBindVertexArray (VAO_Terra);
    identTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    // Paret
    ParetTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if(segonacam) {
        glViewport (0.75*ample, 0.75*alt, ample/4, alt/4);
        ini_camera2();

        colorcam2 = glm::vec3(0.0, 1.0, 0.0);
        glUniform3fv (color2Loc, 1, &colorcam2[0]);

        if(pintarick) {
            pinta = 1;
            glUniform1f(pintaLoc, pinta);
        }

        // Rick
        glBindVertexArray (VAO_models[RICK]);
        RickTransform();
        glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

        if(pintarick) {
            pinta = 0;
            glUniform1f(pintaLoc, pinta);
        }

        // Morty
        glBindVertexArray (VAO_models[MORTY]);
        MortyTransform();
        glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
        
        // VideoCamera
        glBindVertexArray (VAO_models[VIDEOCAMERA]);
        VideoCameraTransform();
        glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);  
        
        // Terra
        glBindVertexArray (VAO_Terra);
        identTransform();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // Paret
        ParetTransform();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glBindVertexArray (0);
}

void MyGLWidget::iniEscena() {
    Pmin = glm::vec3(-4,0,-4);
    Pmax = glm::vec3(4,3,4);

    posMorty = glm::vec3(1,0,0);
    angleMorty = 0;

    psi=0.f;
    theta=M_PI/4;

    angcamY = M_PI/4;

    segonacam = false;
    pintarick = false;
    pinta = 0;

    centreEscena = (Pmin+Pmax) / 2.f;
    radiEscena = (distance(Pmin,Pmax)) / 2.f;
    d = 2*radiEscena;

    activatimer = false;
    canvi = false;
}

void MyGLWidget::iniCamera(){

  //obs = glm::vec3(0, 2, 5);
  //vrp = glm::vec3(0, 1, 0);
  //up = glm::vec3(0, 1, 0);

    fovini = 2.f * glm::asin(radiEscena/d);
    fov = fovini;
    ra = 1.0f;
    znear = d-radiEscena;
    zfar = d+radiEscena;

    vrp = glm::vec3(centreEscena);

  viewTransform();
  projectTransform();
}

void MyGLWidget::viewTransform () {
    glm::mat4 View (1.f);  
    View = glm::translate(View,glm::vec3(0,0,-d));
    View = glm::rotate(View,theta,glm::vec3(1.f,0.f,0.f)); //eix x, moviment dalt/abaix
    View = glm::rotate(View,-psi,glm::vec3(0.f,1.f,0.f)); //eix y, moviment esquerre/dreta
    View = glm::translate(View,-vrp);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform () {
    glm::mat4 Proj(1.0f);
    Proj = glm::perspective (fov, ra, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::resizeGL (int width, int height) {
    ample = width;
    alt = height;
    
    float rav = float(width) / float(height);
    ra = rav;
    
    if(rav<1.f) {
        fov = 2.f * atan(tan(fovini/2.f)/rav); //FOVini = 2 * anglev  -> anglev = FOVini/2
    }
    factorAngleY = M_PI / ample;
    factorAngleX = M_PI / alt;
    projectTransform();
}


void MyGLWidget::RickTransform ()
{
  glm::mat4 TG(1.0f);  
  TG = glm::translate(TG, glm::vec3(-2, 0, -2));
  TG = glm::rotate(TG,float(M_PI),glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
  TG = glm::translate(TG, -centreBaseModels[RICK]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform() {
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posMorty);
  TG = glm::rotate(TG,angleMorty,glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::VideoCameraTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-4, 3, -4));
  TG = glm::rotate(TG,angcamY,glm::vec3(0,1,0));
  TG = glm::rotate(TG,float(M_PI/4),glm::vec3(1,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[VIDEOCAMERA]));
  TG = glm::translate(TG, -centreBaseModels[VIDEOCAMERA]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::ini_camera2() {
    fov2 = float(M_PI/3); //60º
    ra2 = 1.f;
    znear2 = 0.25; //amb znear 0.25 es veia una mica la camera quan girava
    zfar2 = zfar;

    float radicamera = distance(glm::vec3(-4,-3,-4),glm::vec3(0,-3,0));

    float girx = (sin(angcamY)-sin(M_PI/4))*radicamera;
    float girz = (cos(angcamY)-sin(M_PI/4))*radicamera;

    up2 = glm::vec3(sin(angcamY),1,cos(angcamY));
    vrp2 = glm::vec3(girx,-3,girz); 
    obs2 = glm::vec3(-4,3,-4);
    
    viewTransform2();
    projectTransform2();
}

void MyGLWidget::viewTransform2() {
    glm::mat4 View2(1.0f);
    View2 = glm::lookAt (obs2, vrp2, up2);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View2[0][0]);
}

void MyGLWidget::projectTransform2() {
    glm::mat4 Proj2(1.0f);
    Proj2 = glm::perspective (fov2, ra2, znear2, zfar2);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj2[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_Up: { 
            posMorty = posMorty + glm::vec3(sin(angleMorty),0,cos(angleMorty))*glm::vec3(0.1);    	
            break;
        }
        case Qt::Key_Down: {
            posMorty = posMorty - glm::vec3(sin(angleMorty),0,cos(angleMorty))*glm::vec3(0.1);
            break;
        }
        case Qt::Key_Left: { 
            angleMorty += M_PI/4; 	
            break;
        }
        case Qt::Key_Right: { 
            angleMorty -= M_PI/4;  	
            break;
        }
        case Qt::Key_Q: { 
            if(angcamY < M_PI/2) {
                angcamY += M_PI/180;
            }
            break;
        }
        case Qt::Key_E: { 
            if(angcamY > 0) {
                angcamY -= M_PI/180;  
            } 	
            break;
        } 
        case Qt::Key_C: { 
            segonacam = !segonacam; 
            if(segonacam) {
                paintGL();
            }  	
            break;
        }
        case Qt::Key_G: { 
            pintarick = !pintarick;
            break;
        }
        case Qt::Key_R: { 
            iniEscena();
            paintGL();
            creaBuffersModels();
            break;
        }
        case Qt::Key_T: { 
            activatimer = !activatimer;
            if(activatimer) timer.start(1000); //cada segon gira
            else timer.stop();
            break;
        }
        
        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::mouseMoveEvent (QMouseEvent *event) {
    if(DoingInteractive == ROTATE) {
        makeCurrent();

        psi -= factorAngleX*(event->x() - xClick); //eix y, moviment esquerra/dreta (x)
        theta += factorAngleY*(event->y() - yClick); //eix x, moviment baix/dalt (y)
        xClick = event->x();
        yClick = event->y();
        viewTransform();

        update();
    }
}

/*
void MyGLWidget::creaBuffersModels () {
    // Càrrega dels models
  models[RICK].load("./models/Rick.obj");
  models[MORTY].load("./models/Morty.obj");
  models[VIDEOCAMERA].load("./models/VideoCamera.obj");

  // Creació de VAOs i VBOs per pintar els models
  glGenVertexArrays(3, &VAO_models[0]);
  
  float alcadaDesitjada[3] = {2,1,0.25};

  // Calculem la capsa contenidora del model
    calculaCapsaModel (models[0], escalaModels[0], alcadaDesitjada[0], centreBaseModels[0]);

    glBindVertexArray(VAO_models[0]);

    GLuint VBO[2];
    glGenBuffers(2, VBO);

    for (int i = 0; i < 3; i++)
    {	
	  // Calculem la capsa contenidora del model
	  calculaCapsaModel (models[i], escalaModels[i], alcadaDesitjada[i], centreBaseModels[i]);
  
	  glBindVertexArray(VAO_models[i]);

	  GLuint VBO[2];
	  glGenBuffers(2, VBO);

	  // geometria
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_vertices(), GL_STATIC_DRAW);
	  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(vertexLoc);

	  // color
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

      if(i==RICK) {
        if(pintarick) {
            glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 colors[models[i].faces().size()*3*3];

            glm::vec3 c(0.8, 0.7, 1.0);

            for (long unsigned int j = 0; j < models[i].faces().size()*3*3; j+=3) {
                colors[j] = green*c;
                colors[j+1] = green*c;
                colors[j+2] = green*c;
            }

            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
            colors, GL_STATIC_DRAW);
            glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	        glEnableVertexAttribArray(colorLoc); 
        }
        else {
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
            models[i].VBO_matdiff(), GL_STATIC_DRAW);
            glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	        glEnableVertexAttribArray(colorLoc); 
        }
      }
      else {
         glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_matdiff(), GL_STATIC_DRAW);
	    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(colorLoc);
      }  
    }
  
  glBindVertexArray (0);
}
*/

void MyGLWidget::carregaShaders () {
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

    // Identificador per als  atributs
    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    //vertexLoc2 = glGetAttribLocation (program->programId(), "vertex2");
    colorLoc = glGetAttribLocation (program->programId(), "color");

    // Identificadors dels uniform locations
    transLoc = glGetUniformLocation(program->programId(), "TG");
    projLoc  = glGetUniformLocation (program->programId(), "Proj");
    viewLoc  = glGetUniformLocation (program->programId(), "View");
    color2Loc = glGetUniformLocation (program->programId(), "colorcam2");
    pintaLoc = glGetUniformLocation (program->programId(), "pinta");
    }
    
    


