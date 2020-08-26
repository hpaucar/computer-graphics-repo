z#include <iostream>
#include <cstdlib>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader_utils.h"

using namespace std;

GLuint vbo_curve;
GLuint vbo_color;

GLuint program;

GLint attribute_coord2d;
GLint attribute_color;


//Puntos de control
GLfloat cp[4][2]={
	{0.0, 0.0},
	{0.0, 0.5},
	{0.5, -0.5},
	{0.5, 0.5}};

bool init_resources(){

	GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    if((vs = create_shader("basic.v.glsl", GL_VERTEX_SHADER))==0) return false;
    if((fs = create_shader("basic.f.glsl", GL_FRAGMENT_SHADER))==0) return false;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if(!link_ok){
        std::cout << "Problemas con el Shader" << std::endl;
        return false;
    }

    attribute_coord2d = glGetAttribLocation(program, "coord2d");
    if(attribute_coord2d == -1){
        std::cout << "No se puede asociar el atributo coord2d" << std::endl;
        return false;
    }

    attribute_color = glGetAttribLocation(program, "color");
    if(attribute_color == -1){
        std::cout << "No se puede asociar el atributo color" << std::endl;
        return false;
    }
}

void onDisplay(){
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    
    
    glutSwapBuffers();
}

void free_resources(){
	glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_curve);
    glDeleteBuffers(1, &vbo_color);
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitContextVersion(2,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutCreateWindow("OpenGL");

    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK){
        std::cout << "Error inicializando GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!GLEW_VERSION_2_0){
        std::cout << "Tu tarjeta grafica no soporta OpenGL 2.0" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(init_resources()){
        glutDisplayFunc(onDisplay);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutMainLoop();
    }

    free_resources();
    exit(EXIT_SUCCESS);
}
