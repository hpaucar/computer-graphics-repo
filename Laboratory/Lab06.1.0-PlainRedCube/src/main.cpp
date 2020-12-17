//============================================================================
// Name        : Display Plain Red Cube
// Professor   : Herminio Paucar
// Version     :
// Description : Display my first Plain Red Cube
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
float cubeLocX, cubeLocY, cubeLocZ;

GLuint renderingProgram;
GLuint m_VBO;
GLuint m_VAO;

// variable allocation for display
GLuint mLoc, vLoc, projLoc;
int width, height;
float aspect;
glm::mat4 mProjection, mView, mModel;

void setupVertices(void) {
	// Vertex to (2*6) = 12 triangles, (12*3)= 36 points, (36*3) = 108 float values.
	float vertexPositions[108] = {
		//side 01
	 -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, //Triangle 01
	  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, //Triangle 02
	  	//side 02
	  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f, //Triangle 03
	  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, //Triangle 04
	    //side 03
	  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, //Triangle 05
	 -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, //Triangle 06
	    //side 04
	 -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, //Triangle 07
	 -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, //Triangle 08
	    //side 05
	 -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, //Triangle 09
	  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, //Triangle 10
	    //side 06
	 -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, //Triangle 11
	  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f  //Triangle 12
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
}

void init(GLFWwindow *window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = -8.0f;
	cubeLocX = 0.0f;
	cubeLocY = -2.0f;
	cubeLocZ = 0.0f;
	setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
	glUseProgram(renderingProgram);
    // Clear the screen to black
    glClearColor(0.02f, 0.00f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

	// get locations of uniforms in the shader program
	mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
	vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// send matrix data to the uniform variables
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float) width / (float) height;

	/* Matrix configuration to view in 3D */
	mProjection = glm::perspective(
			glm::radians(60.0f), // FOV 1.0472 radians == 60 degrees
			aspect, // width/height
			0.1f, // Near
			100.0f); // Far

	// Camera matrix
	mView = glm::lookAt(
			glm::vec3(cameraX, cameraY, cameraZ), // Camera position in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)

	// Model transformation Rotation
	glm::mat4 tRotat = glm::rotate(
    		glm::mat4(1.0f),
			glm::radians((float)currentTime*50.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)); //Rotate in direction to axis X, Y or Z

	// Model transformation Translate
    glm::mat4 tTransl = glm::translate(
    			glm::mat4(1.0f),
    			glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

    mModel = tTransl*tRotat;

	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mModel));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(mView));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mProjection));

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);  // makes the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associates 0th attribute with buffer
	glEnableVertexAttribArray(0);  // enable the 0th vertex attribute

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glPointSize(10.0);
	//glDrawArrays(GL_POINTS, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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

	GLFWwindow *window = glfwCreateWindow(800, 800, "Lab 06.1: Plain Red Cube",	NULL, NULL);
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
