/*
 * StaticShader.h
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#ifndef SHADERS_STATICSHADER_H_
#define SHADERS_STATICSHADER_H_

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

class StaticShader {
private:
	const char* VERTEX_FILE = NULL;
	const char* FRAGMENT_FILE = NULL;
	GLuint renderingProgram = 0;

public:
	StaticShader();
	virtual ~StaticShader();
	StaticShader(const char* VERTEX_FILE, const char* FRAGMENT_FILE);
	void start();
	void stop();
	void cleanUp();
	void bindAttribute(int attribute, const char* variableName);
};

#endif /* SHADERS_STATICSHADER_H_ */
