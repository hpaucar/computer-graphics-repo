/*
 * Shader.h
 *
 *  Created on: 16 dic. 2020
 *      Author: Usuario
 */

#ifndef SHADER_H_
#define SHADER_H_

#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
// Include GLEW
#define GLEW_STATIC
#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};



#endif /* SHADER_H_ */
