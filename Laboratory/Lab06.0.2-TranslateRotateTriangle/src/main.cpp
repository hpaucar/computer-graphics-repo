//============================================================================
// Name        : Translate X, Y, Diagonal Triangle Object
// Professor   : Herminio Paucar
// Version     :
// Description :
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

GLuint renderingProgram;

GLfloat* m_Vertices;
GLuint n_Vertices;
GLuint m_VBO;
GLuint m_VAO;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;
float curAngle = 0.0f;

void init (GLFWwindow* window) {

	// Utils
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

    // The first 3 points are to Vertex position of Triangle
	m_Vertices = new GLfloat[9] {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};
	n_Vertices = 9;
	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

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

	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	GLuint uniformModel = glGetUniformLocation(renderingProgram, "model");

	if (direction) {
		triOffset += triIncrement;
	} else {
		triOffset -= triIncrement;
	}

	if (std::abs(triOffset) >= triMaxOffset) {
		direction = !direction;
	}

	curAngle += 0.05f;
	if (curAngle >= 360)
	{
		curAngle -= 360;
	}

	// Matriz con elementos de valor 1
	glm::mat4 model(1.0f);
	//Movimiento Horizontal
	model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
	//Movimiento Vertical
	//model = glm::translate(model, glm::vec3(0.0f, triOffset, 0.0f));
	//Movimiento Diagonal
	//model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));

	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

	//Usando UniformMatrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Usando ProgramUniform
	//glProgramUniformMatrix4fv(renderingProgram, uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	// Use este VAO e suas configurações
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glUseProgram(0);
}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab06.0: Translate of Triangle", NULL, NULL);
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
