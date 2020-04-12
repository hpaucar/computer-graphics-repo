/*
 * RawModel.cpp
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#include "RawModel.h"

RawModel::RawModel() {
	// TODO Auto-generated constructor stub

}

RawModel::~RawModel() {
	// TODO Auto-generated destructor stub
}

RawModel::RawModel(int vaoID, int vertexCount) {
	this->vaoID = vaoID;
	this->vertexCount = vertexCount;
}

int RawModel::getVaoID() {
	return vaoID;
}

int RawModel::getVertexCount() {
	return vertexCount;
}
