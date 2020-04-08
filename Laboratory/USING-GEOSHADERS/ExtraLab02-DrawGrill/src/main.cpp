//==============================================================================================
// Name        : Display Grill using GL_LINES primitives with events
// Professor   : Herminio Paucar
// Version     :
// Description : El código utiliza todos los puntos de la grilla, los puntos son considerados elementos.
// Desafio     :
// D1		   : Utilizar otra primitiva para el diseño de una grilla de más de 1000^2 cuadros
//				 usando y sin usar todos los puntos.
// D1		   : Pintar cada cuadrado interno de una color difierente.
//==============================================================================================
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <cmath>

#include "Utils.h"
#include "MyFunctions.h"

using namespace std;
const float  PI_F=3.14159265358979f;

float cameraX, cameraY, cameraZ;

GLuint renderingProgram;
GLuint m_VBO;
GLuint m_VAO;

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, mvMat;

int dimension;
int eleDim;
void setupVertices(void) {

	glGenBuffers(1, &m_VBO);
	//std::vector<glm::vec3> grillxyz = MyFunctions::getGrillQuadPoints(glm::vec3(0, 0, 0), 10, length, "xz");
	glm::vec3 temp, p0(0, 0, 0);
	int radio = 10;
	int length = 1; // 3481;// Max value
	int sz = 2 * length;

	glm::vec3 topLeft(p0.x - 0.5 * radio * (2 * length - 1), p0.y,
			p0.z - 0.5 * radio * (2 * length - 1));
	temp = topLeft;
	int r = sz * sz;
	dimension = r * 6;
	float *points = new float[dimension];
	int c = 0;
	for (int i = 0; i < sz; i++) {
		//grillPoints.push_back(temp);
		points[6 * c + 0] = temp.x;
		points[6 * c + 1] = temp.y;
		points[6 * c + 2] = temp.z;
		points[6 * c + 3] = 0;
		points[6 * c + 4] = 1;
		points[6 * c + 5] = 0;
		c++;
		for (int j = 1; j < sz; j++) {
			temp.x = temp.x + radio;
			//grillPoints.push_back(temp);
			points[6 * c + 0] = temp.x;
			points[6 * c + 1] = temp.y;
			points[6 * c + 2] = temp.z;
			points[6 * c + 3] = 0;
			points[6 * c + 4] = 1;
			points[6 * c + 5] = 0;
			c++;
			//printf("::::>%d (%d, %d) \n",k, (int)temp.x, (int)temp.z);
		}
		temp.x = topLeft.x;
		temp.z = temp.z + radio;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, dimension * sizeof(float), points,
			GL_STATIC_DRAW);

	// Create VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(renderingProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(renderingProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			(void*) (3 * sizeof(GLfloat)));
}

void init(GLFWwindow *window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/geoShader.glsl", "src/fragShader.glsl");

	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 50.0f; // Limit value is far proyection size
	setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
	// Clean windows for each update
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	// get locations of uniforms in the shader program
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// send matrix data to the uniform variables
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float) width / (float) height;
/*
 * glm::perspective(fov, aspect, near, far)
 * See image of material folder in
 * https://knowww.eu/trees?node-id=59b8e93cd54a862e9d7e40df&tree-id=59b8e93cd54a862e9d7e4049#
 */
	pMat = glm::perspective(PI_F/3.0f, aspect, 1.0f, 100000.0f); // 1.0472 radians == 60 degrees
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// Move FRONT
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraZ += 0.5f;
	// Move BACK
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraZ -= 0.5f;
	// Move RIGHT
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraX += 0.5f;
	// Move LEFT
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraX -= 0.5f;
	// Move UP
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraY += 0.5f;
	// Move DOWN
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraY -= 0.5f;

	mvMat = glm::translate(
				glm::mat4(1.0f),
				glm::vec3(-cameraX, -cameraY, -cameraZ));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(m_VAO);
    glDrawArrays(GL_POINTS, 0, dimension/6);
	glBindVertexArray(0);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	GLFWwindow *window = glfwCreateWindow(800, 800, "Lab 06.1.3: Draw Grill using points", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
