//=====================================================================================
// Name        : Event Capture (KEY_SCAPE, KEY_LEFT, KEY_RIGHT, KEY_UP and KEY_DOWN)
// Professor   : Herminio Paucar
// Version     :
// Description :
//=====================================================================================

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

float x = 0.0f;
float y = 0.0f;

using namespace std;

void init(GLFWwindow *window) {

	// Utils
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
}

void display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	// Move RIGHT
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		x += 0.01f;
	// Move LEFT
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		x += -0.01f;
	// Move UP
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		y += 0.01f;
	// Move DOWN
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		y += -0.01f;

	glm::vec2 newPos(x, y);

	//Obtiene el valor de la posición(No su valor) de la variable uniforme "offset"
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
	//void glProgramUniform2fv(	GLuint program,	GLint location,	GLsizei count, 	const GLfloat *value);
	glProgramUniform2fv(renderingProgram, offsetLoc, 1, glm::value_ptr(newPos));
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

	GLFWwindow *window = glfwCreateWindow(800, 800, "Lab05.3: Event Capture", NULL,	NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window) && // Presionamos el boton (x) salir
			glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) // Presionamos la tecla scape
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
