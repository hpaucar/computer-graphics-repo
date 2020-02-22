// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

void init(GLFWwindow *window) {}

void display(GLFWwindow *window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	float vertices[] = {
			0.0, 0.5, 0.0, // top corner
			-0.5, -0.5, 0.0, // bottom left corner
			0.5, -0.5, 0.0 // bottom right corner
			};
	// Render OpenGL here
	// tell OpenGL that you're using a vertex array for fixed-function attribute
	glEnableClientState(GL_VERTEX_ARRAY);
	// point to the vertices to be used
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	// draw the vertixes
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// tell OpenGL that you're finished using the vertex arrayattribute
	glDisableClientState(GL_VERTEX_ARRAY);
}

int main(void) {
	// Initialize the library
	if (!glfwInit()) {
		return -1;
	}

	// Create a windowed mode window and its OpenGL context
	GLFWwindow *window = glfwCreateWindow(640, 480, "First Triangle", NULL,
			NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		// includes a reference to the GLFW windows object and the current time
		display(window, glfwGetTime());
		// Swap front and back buffers
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
