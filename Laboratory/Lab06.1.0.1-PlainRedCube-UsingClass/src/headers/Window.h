/*
 * Window.h
 *
 *  Created on: 16 dic. 2020
 *      Author: Usuario
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#pragma once

#include "stdio.h"
// Include GLEW
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

#endif /* WINDOW_H_ */
