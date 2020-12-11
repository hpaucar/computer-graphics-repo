//============================================================================
// Name        :
// Professor   : Herminio Paucar
// Version     :
// Description : Utilizamos los Vertex y Fragment Shaders
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

#define W_WIDTH 800
#define W_HEIGHT 800

GLuint renderingProgram;

GLfloat* m_Vertices;
GLuint n_Vertices;
GLuint m_VBO;
GLuint m_VAO;

using namespace std;

void init (GLFWwindow* window) {

    //renderingProgram = createShaderProgram();
	// Utils
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

    // The first 3 points are to Vertex position of Triangle
    n_Vertices = 18;
    m_Vertices = new GLfloat[18] {
			-1.0f, -1.0f, 0.0f, //Triangle 01
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, //Triangle 02
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f
	};

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			18 * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Política de acesso aos dados, para otimização
		);

	// Nesse ponto, ja copiamos nossos dados pra GPU.
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(GLvoid*) 0
		);

	glEnableVertexAttribArray(0);	// Habilita este atributo

	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    //Obtiene el id de la variable uniforme "u_time"
    GLuint uTime = glGetUniformLocation(renderingProgram, "u_time");
    //Se va actualizar un vector de 2 valores
	glProgramUniform1f(renderingProgram, uTime, currentTime);

	// Use este VAO e suas configurações
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
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

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "LabShader02.1: Display Change Color Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    init(window);
    while (!glfwWindowShouldClose(window)) {

        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
