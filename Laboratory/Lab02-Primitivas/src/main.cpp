//============================================================================
// Name        : Lab 02 Primitives
// Professor   : Herminio Paucar
// Version     :
// Description : Drawing all primitives
//============================================================================
// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

using namespace std;
// Vertex Array Object
GLuint m_VAO;

//  Vertex Buffer Object
GLuint m_VBO;

int dimVertices;
int numberOfVertices;

void init(){
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create vertices
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	GLfloat radius = 1;
	GLint numberOfSides = 50; // 50
	numberOfVertices = numberOfSides+1; // points + one center point.
	GLfloat twicePi = 2.0f * M_PI;

	GLfloat verticesX[numberOfVertices];
	GLfloat verticesY[numberOfVertices];
	GLfloat verticesZ[numberOfVertices];

	verticesX[0] = x;
	verticesY[0] = y;
	verticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++) {
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
		//cout<<" P["<<i<<"]"<<": ("<<verticesX[i]<<","<<verticesY[i]<<","<<verticesZ[i]<<") \n";
	}
	dimVertices = (numberOfVertices) * 3;
	GLfloat m_Vertices[dimVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices[i * 3] = verticesX[i];
		m_Vertices[i * 3 + 1] = verticesY[i];
		m_Vertices[i * 3 + 2] = verticesZ[i];
		//cout<<" P["<<i<<"]"<<": ("<<m_Vertices2[i * 3]<<","<<m_Vertices2[i * 3+1]<<","<<m_Vertices2[i * 3+2]<<") \n";
	}

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(
			GL_ARRAY_BUFFER,
			dimVertices * sizeof(GLfloat),
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
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(
			0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*) 0            // array buffer offset
	);
	//glPointSize(10.0f);
	/**** PRIMITIVAS DE OPENGL ******/
	GLenum mode[7] = {
				GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
				GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP };

	// Draw the primitives !
	glDrawArrays(mode[(int)currentTime%7], 0, numberOfVertices); // 3 indices starting at 0 -> number of Vertex

	glDisableVertexAttribArray(0);
}

int main( void )
 {
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
	window = glfwCreateWindow(800, 800, "Tutorial: Draw Primitives", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

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

