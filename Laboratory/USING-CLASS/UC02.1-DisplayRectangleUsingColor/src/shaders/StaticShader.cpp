/*
 * StaticShader.cpp
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#include "StaticShader.h"
#include "Utils.h"


StaticShader::StaticShader() {
	// TODO Auto-generated constructor stub

}

StaticShader::~StaticShader() {
	// TODO Auto-generated destructor stub
}

StaticShader::StaticShader(const char *VERTEX_FILE, const char *FRAGMENT_FILE) {
	this->VERTEX_FILE = VERTEX_FILE;
	this->FRAGMENT_FILE = FRAGMENT_FILE;

	renderingProgram = Utils::createShaderProgram(VERTEX_FILE, FRAGMENT_FILE);
	//bindAttribute(0, "iPosition");
	//bindAttribute(1, "iColor");
	//std::cout<<"RP: "<< renderingProgram<<"\n";
}

void StaticShader::start() {
	glUseProgram(renderingProgram);
}

void StaticShader::stop() {
	glUseProgram(0);
}

void StaticShader::cleanUp() {
	stop();
	//glDetachShader(programID, vertexShaderID);
	//glDetachShader(programID, fragmentShaderID);
	//glDeleteShader(vertexShaderID);
	//glDeleteShader(fragmentShaderID);
	glDeleteProgram(renderingProgram);
}

void StaticShader::bindAttribute(int attribute, const char *variableName) {
	glBindAttribLocation(renderingProgram, attribute, variableName);
}

