/*
 * Renderer.h
 *
 *  Created on: 10 abr. 2020
 *      Author: Usuario
 */

#ifndef RENDERENGINE_RENDERER_H_
#define RENDERENGINE_RENDERER_H_
#include "RawModel.h"
// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

	void prepare();
	void render(RawModel model);

};

#endif /* RENDERENGINE_RENDERER_H_ */
