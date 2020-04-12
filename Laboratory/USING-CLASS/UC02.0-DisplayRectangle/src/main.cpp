#include <iostream>
#include <memory>

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"
#include "shaders/StaticShader.h"
#include "renderEngine/RawModel.h"

int main(int argc, char** argv){
	
	// Create Context to display
	DisplayManager dm(800, 800, "First theme");
	dm.createDisplay();

	// Crate Loader to
	Loader loader;
	// Create Renderer to
	Renderer renderer;

	// Create shader
	StaticShader shader("src/shaders/vertShader.glsl", "src/shaders/fragShader.glsl");

	float* vertices = new float[12]{
		-0.5f, 0.5f, 0,	//V0
		-0.5f,-0.5f, 0,	//V1
		 0.5f,-0.5f, 0,	//V2
	 	 0.5f, 0.5f, 0	//V3
	};

	int* indices = new int[6]{
		0, 1, 3, //Top left triangle (V0,V1,V3)
		3, 1, 2	 //Bottom right triangle (V3,V1,V2)
	};

	RawModel model = loader.loadToVAO(vertices, 12, indices, 6);

	while (!dm.isClosed()) {
		// game logic
		// render geometry
		renderer.prepare();
		shader.start();
		renderer.render(model);
		shader.stop();
		dm.updateDisplay();
	}

	shader.cleanUp();
	loader.cleanUp();
	dm.closeDisplay();

	return 0;
}
