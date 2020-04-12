/*
 * RawModel.h
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#ifndef RENDERENGINE_RAWMODEL_H_
#define RENDERENGINE_RAWMODEL_H_

class RawModel {

private:
	int vaoID = 0;
	int vertexCount = 0;

public:
	RawModel();
	virtual ~RawModel();

	RawModel(int vaoID, int vertexCount);
	int getVaoID();
	int getVertexCount();

};

#endif /* RENDERENGINE_RAWMODEL_H_ */
