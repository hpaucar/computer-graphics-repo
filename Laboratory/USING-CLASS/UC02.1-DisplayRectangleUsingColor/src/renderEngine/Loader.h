/*
 * Loader.h
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#ifndef RENDERENGINE_LOADER_H_
#define RENDERENGINE_LOADER_H_
#include "RawModel.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

class Loader {
private:
	void createVAO();
	void storeDataInAttributeList();
	void unbindVAO();
	void bindIndicesBuffer(int* indices);
	void storeDataInFloatBuffer(GLuint *m_VBO, float *data, int nd);
	void storeDataInIntBuffer(GLuint *m_EBO, int *data, int nd);
public:
	Loader();
	virtual ~Loader();

	RawModel loadToVAO(float *positions, int np, int *indices, int ni);
	void cleanUp();

};

#endif /* RENDERENGINE_LOADER_H_ */
