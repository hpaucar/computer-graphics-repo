//============================================================================
// Name        : Knowing the Camera functions
// Professor   : Herminio Paucar
// Version     :
// Description : Configure my Camera
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
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"

using namespace std;

float cameraX, cameraY, cameraZ;

GLuint renderingProgram;
GLuint m_VBO;
GLuint m_VAO;

// variable allocation for display
int width, height;

void setupVertices(void) {
	// Vertex to (1*6) = 2 triangles.
	float vertexPositions[9] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
 		 0.0f,  0.5f, 0.0f
	};

	glGenVertexArrays(1, &m_VAO);// creates VAO and returns the integer ID
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO); // creates VBO and returns the integer ID
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(vertexPositions),
			vertexPositions,
			GL_STATIC_DRAW);

	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(GLfloat),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);	// Habilita este atributo Layout 0
}

void init(GLFWwindow *window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
	//position of camera
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 20.0f;
	setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram);

	// get locations of uniforms in the shader program
	GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// send matrix data to the uniform variables
	glfwGetFramebufferSize(window, &width, &height);
	GLfloat aspect = (float) width / (float) height;

	glm::mat4 pMat = glm::perspective((float)M_PI/3, aspect, 0.1f, 1000.0f); // 1.0472 radians == 60 degrees
	glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);  // makes the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associates 0th attribute with buffer
	glEnableVertexAttribArray(0);  // enable the 0th vertex attribute

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glPointSize(10.0);
	//glDrawArrays(GL_POINTS, 0, 3);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	GLFWwindow *window = glfwCreateWindow(800, 800, "Lab 06.0.5: Camera",	NULL, NULL);
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
