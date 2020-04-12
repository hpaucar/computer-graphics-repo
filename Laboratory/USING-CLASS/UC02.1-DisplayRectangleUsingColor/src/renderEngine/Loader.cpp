/*
 * Loader.cpp
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */
#include <iostream>
#include "Loader.h"

Loader::Loader() {
	// TODO Auto-generated constructor stub

}

Loader::~Loader() {
	// TODO Auto-generated destructor stub
}

void Loader::createVAO() {

}
void Loader::storeDataInAttributeList() {
	// Specify the layout of the vertex data Position
	glVertexAttribPointer(
			0,//posAttrib,
			2,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GLfloat),
			0);
	glEnableVertexAttribArray(0); //Habilitamos el Layout de valor 0 posicion

	// Specify the layout of the vertex data Color
	glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GLfloat),
			(void*) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); //Habilitamos el Layout de valor 1 color
}
void Loader::unbindVAO() {

}
void Loader::bindIndicesBuffer(int *indices) {

}
void Loader::storeDataInFloatBuffer(GLuint *m_VBO, float *data, int nd) {
	//GLuint m_VBO;
	glGenBuffers(1, m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *m_VBO);
		// Reserva memoria na GPU para um TARGET receber dados
		// Copia esses dados pra essa área de memoria
	glBufferData(
		GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
		nd * sizeof(GLfloat),	// tamanho do buffer
		data,			// Dados a serem copiados pra GPU
		GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização
}
void Loader::storeDataInIntBuffer(GLuint *m_EBO, int *data, int nd) {
    // Create an element array
	//GLuint m_EBO;
    glGenBuffers(1, m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		nd * sizeof(int),
		data,
		GL_STATIC_DRAW);
}
RawModel Loader::loadToVAO(float *positions, int np, int *indices, int ni) {
    // Create Vertex Array Object
	GLuint m_VAO, m_VBO, m_EBO;

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

	storeDataInFloatBuffer(&m_VBO, positions, np);
	storeDataInIntBuffer(&m_EBO, indices, ni);

	storeDataInAttributeList();

	//std::cout<<" :::> "<<m_VAO<<" ; "<<m_VBO<<" ; "<<m_EBO<<"\n";
	// Return: :::> 1 ; 1 ; 2
	return RawModel(m_VAO, ni);
}

void Loader::cleanUp() {

}
