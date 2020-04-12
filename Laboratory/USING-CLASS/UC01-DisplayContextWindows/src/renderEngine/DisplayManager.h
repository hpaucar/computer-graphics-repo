/*
 * DisplayManager.h
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#ifndef RENDERENGINE_DISPLAYMANAGER_H_
#define RENDERENGINE_DISPLAYMANAGER_H_
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>



class DisplayManager {

private:
	int WIDTH = 1280;
	int HEIGHT = 720;
	int FPS_CAP = 60;
	GLFWwindow* window = NULL;
	const char* TITLE = "None Title";

	long lastFrameTime = 0;
//	float delta;
public:
	DisplayManager();
	virtual ~DisplayManager();

	DisplayManager(int width, int height, const char* title);
	void createDisplay();
	void updateDisplay();
	void closeDisplay();
	bool isClosed();

};

#endif /* RENDERENGINE_DISPLAYMANAGER_H_ */
