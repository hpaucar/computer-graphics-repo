#include <iostream>
#include <memory>

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"
#include "shaders/StaticShader.h"
#include "renderEngine/RawModel.h"

int main(int argc, char** argv){
	
	// Create Context to display
	DisplayManager dm(800, 800, "First theme UC- Rectangle");
	// Create Windows and verify the funcions of GLFW and GLEW
	dm.createDisplay();

	// Crate Loader to
	Loader loader;
	// Create Renderer to
	Renderer renderer;

	// Create shader and rendering Program
	StaticShader shader("src/shaders/vertShader.glsl", "src/shaders/fragShader.glsl");

	float* vertices = new float[20]{
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	int* indices = new int[6]{
		0, 1, 3, //Top left triangle (V0,V1,V3)
		3, 1, 2	 //Bottom right triangle (V3,V1,V2)
	};

	// Create VAO, VBO and add vertices to VertexPointer
	RawModel model = loader.loadToVAO(vertices, 20, indices, 6);

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
