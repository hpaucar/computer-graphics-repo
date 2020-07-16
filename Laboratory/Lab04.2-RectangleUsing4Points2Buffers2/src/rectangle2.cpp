//============================================================================
// Name        : Draw Rectangle using four points and Two Buffers (Elements Buffers).
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

GLuint renderingProgram;

GLfloat* m_Vertices;

using namespace std;

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
    // Create Vertex Array Object
	GLuint m_VAO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint m_VBO;
    glGenBuffers(1, &m_VBO);
	m_Vertices = new GLfloat[20] {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			20 * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização

    // Create an element array
    GLuint m_EBO;
    glGenBuffers(1, &m_EBO);
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(elements),
			elements,
			GL_STATIC_DRAW);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(renderingProgram, "iPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(
			posAttrib,
			2,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GLfloat),
			0);

	GLint colAttrib = glGetAttribLocation(renderingProgram, "iColor");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(
			colAttrib,
			3,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GLfloat),
			(void*) (2 * sizeof(GLfloat)));
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab04.1: Draw Rectangle using 4 points", NULL, NULL);
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
