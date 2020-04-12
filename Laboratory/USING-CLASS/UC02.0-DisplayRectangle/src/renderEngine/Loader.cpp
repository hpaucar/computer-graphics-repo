/*
 * Loader.cpp
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */
// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "Loader.h"

Loader::Loader() {
	// TODO Auto-generated constructor stub

}

Loader::~Loader() {
	// TODO Auto-generated destructor stub
}

void Loader::createVAO() {

}
void Loader::storeDataInAttributeList(int attributeNumber, float *data) {
	// Specify the layout of the vertex data
	glVertexAttribPointer(
			1,// Layout 1 del valor asignado en el Vertex Shader,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(GLfloat),
			0);
	glEnableVertexAttribArray(1); //Habilitamos el Layout de valor 1
}
void Loader::unbindVAO() {

}
void Loader::bindIndicesBuffer(int *indices) {

}
void Loader::storeDataInFloatBuffer(float *data, int nd) {
	GLuint m_VBO;
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		// Reserva memoria na GPU para um TARGET receber dados
		// Copia esses dados pra essa área de memoria
	glBufferData(
		GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
		nd * sizeof(GLfloat),	// tamanho do buffer
		data,			// Dados a serem copiados pra GPU
		GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização
}
void Loader::storeDataInIntBuffer(int *data, int nd) {
    // Create an element array
	GLuint m_EBO;
    glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		nd * sizeof(int),
		data,
		GL_STATIC_DRAW);
}
RawModel Loader::loadToVAO(float *positions, int np, int *indices, int ni) {
    // Create Vertex Array Object
	GLuint m_VAO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

	storeDataInFloatBuffer(positions, np);
	storeDataInIntBuffer(indices, ni);

	storeDataInAttributeList(3, positions);

	return RawModel(m_VAO, ni);
}

void Loader::cleanUp() {

}
