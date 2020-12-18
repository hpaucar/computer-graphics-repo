/*
 * Mesh.h
 *
 *  Created on: 16 dic. 2020
 *      Author: Usuario
 */

#ifndef MESH_H_
#define MESH_H_

#pragma once
// Include GLEW
#define GLEW_STATIC
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateMesh(GLfloat *vertices, unsigned int numOfVertices);

	void RenderMesh();
	void RenderMeshMultiplyInstance(GLuint instances);
	void ClearMesh();
	void printVAO_VBO_EBO();
	~Mesh();

private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
};



#endif /* MESH_H_ */
