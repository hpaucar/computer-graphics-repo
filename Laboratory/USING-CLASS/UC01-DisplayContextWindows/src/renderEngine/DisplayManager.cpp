/*
 * DisplayManager.cpp
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#include "DisplayManager.h"

DisplayManager::DisplayManager() {
	// TODO Auto-generated constructor stub

}

DisplayManager::~DisplayManager() {
	// TODO Auto-generated destructor stub
}

DisplayManager::DisplayManager(int width, int height, const char *title) {
	this->WIDTH = width;
	this->HEIGHT = height;
	this->TITLE = title;
}

void DisplayManager::createDisplay() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 	// Resizable option.

	window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	// Mostra dados do ambiente
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	std::cout << "Renderizador: " << renderer << std::endl;
	std::cout << "Version de OpenGL soportada: " << version << std::endl;
}

void DisplayManager::updateDisplay() {
	lastFrameTime = glfwGetTime();
	// background
	float m = ((int) lastFrameTime % 2 == 0) ? 0.0f : 1.0f;
	glClearColor(1.0f, m, 0.0f, 0.0f);
	//glClearColor(1.0f, 1.0f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool DisplayManager::isClosed() {
	return glfwWindowShouldClose(window);
}

void DisplayManager::closeDisplay() {
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
