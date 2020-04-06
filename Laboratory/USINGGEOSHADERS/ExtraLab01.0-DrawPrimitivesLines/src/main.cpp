//============================================================================
// Name        : Draw Primitive using Geometry Shaders
// Professor   : Herminio Paucar.
// Version     :
// Description :
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

GLfloat* m_Vertices;
GLuint renderingProgram;
using namespace std;

// Shader creation helper
GLuint createShader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/geoShader.glsl", "src/fragShader.glsl");

	// Create VBO with point coordinates
	GLuint vbo;
	glGenBuffers(1, &vbo);

    GLfloat points[] = {
    //  Coordinates (2)     Color (3)
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,
         0.45f,  0.45f, 0.0f, 1.0f, 0.0f,
         0.45f, -0.45f, 0.0f, 0.0f, 1.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f
    };

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(renderingProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(renderingProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			(void*) (2 * sizeof(GLfloat)));
}

void display(GLFWwindow* window, double currentTime) {
	 glUseProgram(renderingProgram);
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Render frame
    glDrawArrays(GL_POINTS, 0, 4);
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "ExtraLab01.0: Draw using Geometry Shaders", NULL, NULL);
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
