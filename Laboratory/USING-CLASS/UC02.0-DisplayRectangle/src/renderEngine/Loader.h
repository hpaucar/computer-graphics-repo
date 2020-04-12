/*
 * Loader.h
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#ifndef RENDERENGINE_LOADER_H_
#define RENDERENGINE_LOADER_H_
#include "RawModel.h"


class Loader {
private:
	void createVAO();
	void storeDataInAttributeList(int attributeNumber, float* data);
	void unbindVAO();
	void bindIndicesBuffer(int* indices);
	void storeDataInFloatBuffer(float* data,  int nd);
	void storeDataInIntBuffer(int *data, int nd);
public:
	Loader();
	virtual ~Loader();

	RawModel loadToVAO(float *positions, int np, int *indices, int ni);
	void cleanUp();

};

#endif /* RENDERENGINE_LOADER_H_ */
