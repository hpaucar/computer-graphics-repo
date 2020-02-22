// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <math.h>

#define SCREEN_W 640
#define SCREEN_H 480

/* send values from display(SCREEN_W/2, SCREEN_H/2, 0, 120, 10) */
void display(GLFWwindow *window, double currentTime) {
	GLfloat x = SCREEN_W / 2;
	GLfloat y = SCREEN_H / 2;
	GLfloat z = 0;
	GLfloat radius = 120;
	GLint numberOfSides  = (int)currentTime;
	int numberOfVertices = numberOfSides + 2;
	GLfloat twicePi = 2.0f * M_PI;

	GLfloat circleVerticesX[numberOfVertices];
	GLfloat circleVerticesY[numberOfVertices];
	GLfloat circleVerticesZ[numberOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++) {
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}
	GLfloat allCircleVertices[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++) {
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[i * 3 + 1] = circleVerticesY[i];
		allCircleVertices[i * 3 + 2] = circleVerticesZ[i];
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

int main(void) {
	// Initialize the library
	if (!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context
	GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Draw Circle",
	NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	// specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glViewport(0.0f, 0.0f, SCREEN_W, SCREEN_H);
	// projection matrix defines the properties of the camera that views the
	// objects in the world coordinate frame. Here you typically set the zoom
	// factor, aspect ratio and the near and far clipping planes
	glMatrixMode(GL_PROJECTION);
	// replace the current matrix with the identity matrix and starts us a
	// fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glLoadIdentity();
	// essentially set coordinate system
	glOrtho(0, SCREEN_W, 0, SCREEN_H, 0, 1);
	// (default matrix mode) modelview matrix defines how your objects
	// are transformed (meaning translation, rotation and scaling) in your world
	glMatrixMode(GL_MODELVIEW);
	// same as above comment
	glLoadIdentity();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		glClear( GL_COLOR_BUFFER_BIT);
		// render OpenGL here
		display(window, glfwGetTime());
		// Swap front and back buffers
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
