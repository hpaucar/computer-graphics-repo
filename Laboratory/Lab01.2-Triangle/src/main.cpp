//============================================================================
// Name        : Program 1.2, Drawing Triangle
// Professor   : Herminio Paucar
// Description : Display my first Triangle
//============================================================================
// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

using namespace std;

GLuint m_VAO;
GLuint m_VBO;

int totalCoordenadas;
int numberOfVertices;

void init(){
	// Create vertices
	numberOfVertices = 3;
 	totalCoordenadas = numberOfVertices * 3;

 	GLfloat m_Vertices [] = {
		-1.0f, -1.0f, 0.0f, //v1
		 1.0f, -1.0f, 0.0f, //v2
		 0.0f,  1.0f, 0.0f //v3
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(
			GL_ARRAY_BUFFER,
			totalCoordenadas * sizeof(GLfloat),
			m_Vertices,
			GL_STATIC_DRAW
	);
}

void display(double currentTime) {
	// Clear the screen
	// Dark blue background
	//float m = ((int) currentTime % 2 == 0) ? 0.0f : 1.0f;
	//glClearColor(1.0f, m, 0.0f, 0.0f);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear( GL_COLOR_BUFFER_BIT);

	// 1rst attribute buffer : vertices

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(
			0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
			numberOfVertices,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*) 0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);
}

int main( void ){
	// Initialise GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 800, "Lab 01.2 - Draw triangle", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	init();

	// Check if the window was closed
	while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup VBO
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

