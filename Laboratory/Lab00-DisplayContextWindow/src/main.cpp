//============================================================================
// Name        : Program 1, Drawing my first Window
// Professor   : Herminio Paucar
// Description : Aplicacion que muestra un cuadro pintado con amarillo.
// Usamos las funciones de contexto propios de GLEW y GLFW
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

GLFWwindow *window;

using namespace std;

int dimVertices;
int numberOfVertices;

void display(double currentTime) {
	// Clear the screen
	// background
	float m = ((int) currentTime % 2 == 0) ? 0.0f : 1.0f;
	glClearColor(1.0f, m, 0.0f, 0.0f);
	//glClearColor(1.0f, 1.0f, 0.5f, 0.0f);
	glClear( GL_COLOR_BUFFER_BIT);
}

int main( void ){
	// Initialise GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	// Version of Modern OpenGL, example: 3.3
	// First 3 is VERSION_MAJOR
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Second 3 is VERSION_MINOR
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Resizable option of Windows.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Open a window and create its OpenGL context
	// parameters width, height, title, monitor and share
	window = glfwCreateWindow(800, 800, "Lab00: Display Context and Window", NULL, NULL);

	// Make windows to Context Current
	glfwMakeContextCurrent(window);

	// Verify and Initialize GLEW
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	//Display environment's informations
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout<<"Renderizador: "<<renderer<<endl;
	cout<<"Version de OpenGL configurada: "<<version<<endl;

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

