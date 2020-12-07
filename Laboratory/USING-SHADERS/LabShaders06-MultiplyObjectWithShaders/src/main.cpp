//============================================================================
// Name        : Display Animation using Shaders
// Professor   : Herminio Paucar
// Version     :
// Description : Display my first Animation
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

#include "Utils.h"

GLuint renderingProgram;
GLfloat *m_Vertices;
GLuint n_Vertices;
GLuint m_VBO;
GLuint m_VAO;

using namespace std;

void init() {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

    // The first 3 points are to Vertex position of Triangle
	m_Vertices = new GLfloat[9] {
		-0.3f, -0.3f, 0.0f,
		 0.3f, -0.3f, 0.0f,
		 0.0f,  0.6f, 0.0f
	};
	n_Vertices = 9;
	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			n_Vertices * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Política de acesso aos dados, para otimização
		);

	glVertexAttribPointer(
			0,			// Lembra do (layout = 0 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice é composto de 3 valores
			GL_FLOAT,	// cada valor do vértice é do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, já esta tudo correto, então deixamos como FALSE)
			0 * sizeof(GLfloat),// De quantos em quantos bytes, este atributo é encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*) 0	// Onde está o primeiro valor deste atributo no buffer. Nesse caso, está no início do buffer
		);
	glEnableVertexAttribArray(0);	// Habilita este atributo
}

void display(double currentTime) {
    glUseProgram(renderingProgram);
    
    // clear the background to black, each time
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // get ptr to "rotate"
    GLuint rotateLoc = glGetUniformLocation(renderingProgram, "rotate");
    
    glProgramUniform1f(renderingProgram, rotateLoc, currentTime);
    // Use este VAO e suas configurações
    glBindVertexArray(m_VAO);
    //Draw Triangles using Instances
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 12);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "LabShaders06.1: Multiply Object with Shaders", NULL, NULL);
    glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
    glfwSwapInterval(1);
    
    init();
    while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
