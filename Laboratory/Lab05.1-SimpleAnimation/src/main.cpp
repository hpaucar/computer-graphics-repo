//============================================================================
// Name        : Display Animation using Shaders
// Professor   : Herminio Paucar
// Version     :
// Description : Display my first Animation
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

#include "Utils.h"

GLuint renderingProgram;
GLuint m_VBO;
GLuint m_VAO;

// location of triangle on x axis
float x = 0.0f;
// offset for moving the triangle
float inc = 0.01f;

using namespace std;

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);
    
    // clear the background to black, each time
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // move the triangle along x axis
    x += inc;
    // switch to moving the triangle to the left
    if (x > 1.0f) inc = -0.01f;
    // switch to moving the triangle to the right
    if (x < -1.0f) inc = 0.01f;
    
    // get ptr to "offset"
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    
    glProgramUniform1f(renderingProgram, offsetLoc, x);
    //Draw Triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //Draw Rectangle with Two Triangles, active 6 vertex positions in VertexShaders
    //glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab05.1: Display First Animation", NULL, NULL);
    glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
    glfwSwapInterval(1);
    
    init(window);
    
    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
