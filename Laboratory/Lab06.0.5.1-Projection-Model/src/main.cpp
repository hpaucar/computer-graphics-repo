//============================================================================
// Name        : Knowing the 3D Viewing: Projection + Model
// Professor   : Herminio Paucar
// Version     :
// Description : Configure Projection and Model
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
#include <glm/ext.hpp> // Print GLM matrix.
#include <glm/gtx/string_cast.hpp>
#include "Utils.h"

using namespace std;

GLuint renderingProgram;
GLuint m_VBO;
GLuint m_VAO;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VBO, VAO, EBO, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

glm::mat4 projection;

void setupVertices() {
	// Figura Tetraedro
	unsigned int indices[] = {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2 };

	GLfloat vertices[] = {
			-1.0f, -1.0f, 0.0f,
			 0.0f, -1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
			GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void calculateProjection(GLFWwindow *mainWindow) {
	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
	GLfloat aspect = (GLfloat) bufferWidth / (GLfloat) bufferHeight;
	projection = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 100.0f);
}

void init(GLFWwindow *mainWindow) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
	setupVertices();
	calculateProjection(mainWindow);
}

void display() {
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(renderingProgram);

	// control in horizontal direction
	if (direction)
		triOffset += triIncrement;
	else
		triOffset -= triIncrement;

	if (abs(triOffset) >= triMaxOffset)
		direction = !direction;

	// control in angle 360
	curAngle += 0.01f;
	if (curAngle >= 360)
		curAngle -= 360;

	if (direction)
		curSize += 0.0001f;
	else
		curSize -= 0.0001f;

	if (curSize >= maxSize || curSize <= minSize)
		sizeDirection = !sizeDirection;


	uniformModel = glGetUniformLocation(renderingProgram, "model");
	uniformProjection = glGetUniformLocation(renderingProgram, "projection");

	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

int main() {
	// Initialise GLFW
	if (!glfwInit()) {
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Lab06.0.5.1 Projection-Model", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	init(mainWindow);


	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get + Handle user input events
		glfwPollEvents();
		display();
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
