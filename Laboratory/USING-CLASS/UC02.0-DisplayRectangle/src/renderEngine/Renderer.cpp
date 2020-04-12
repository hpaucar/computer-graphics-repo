/*
 * Renderer.cpp
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#include "Renderer.h"

Renderer::Renderer() {
	// TODO Auto-generated constructor stub

}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

void Renderer::prepare() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);
}

void Renderer::render(RawModel model) {
	glBindVertexArray(model.getVaoID());
	glEnableVertexAttribArray(0);

	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	glUseProgram(3);
}
