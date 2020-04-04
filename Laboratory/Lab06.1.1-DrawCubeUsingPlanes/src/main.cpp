//============================================================================
// Name        : Draw Cube using Planes XY, XZ and YZ
// Professor   : Herminio Paucar
// Version     :
// Description : Display my first Plain Red Cube
//============================================================================

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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

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

void setupVertices(void) {
	std::vector<Point> xyzrgb;
	// P0: Punto inicial de donde se graficaran los rectangulos
	glm::vec3 mPoint = glm::vec3(-10.0f, 10.0f, 0.0f);
	// First 3 sides
	std::vector<Point> xy = MyFunctions::getRectanglePointsColor(mPoint, 20.0f, 20.0f, "xy", glm::vec3(1.0f, 0.0f, 0.0f));
	std::vector<Point> xz = MyFunctions::getRectanglePointsColor(mPoint, 20.0f, 20.0f, "xz", glm::vec3(0.0f, 1.0f, 0.0f));
	std::vector<Point> yz = MyFunctions::getRectanglePointsColor(mPoint, 20.0f, 20.0f, "yz", glm::vec3(0.0f, 0.0f, 1.0f));
	xyzrgb.insert(xyzrgb.begin(), xy.begin(), xy.end());
	xyzrgb.insert(xyzrgb.begin(), xz.begin(), xz.end());
	xyzrgb.insert(xyzrgb.begin(), yz.begin(), yz.end());
	// Second 2 sides
	glm::vec3 mPoint2 = glm::vec3(-10.0f, -10.0f, 0.0f);
	std::vector<Point> xz2 = MyFunctions::getRectanglePointsColor(mPoint2, 20.0f, 20.0f, "xz", glm::vec3(0.5f, 1.0f, 0.7f));
	glm::vec3 mPoint3 = glm::vec3(10.0f, 10.0f, 0.0f);
	std::vector<Point> yz2 = MyFunctions::getRectanglePointsColor(mPoint3, 20.0f, 20.0f, "yz", glm::vec3(0.5f, 0.5f, 1.0f));
	xyzrgb.insert(xyzrgb.begin(), xz2.begin(), xz2.end());
	xyzrgb.insert(xyzrgb.begin(), yz2.begin(), yz2.end());

	dimension = xyzrgb.size()*6; // 6 value for each point
	float vxyz[dimension];
	for(unsigned int i=0; i< xyzrgb.size(); i++){
		vxyz[6*i+0] = xyzrgb[i].coord.x;
		vxyz[6*i+1] = xyzrgb[i].coord.y;
		vxyz[6*i+2] = xyzrgb[i].coord.z;
		vxyz[6*i+3] = xyzrgb[i].color.r;
		vxyz[6*i+4] = xyzrgb[i].color.g;
		vxyz[6*i+5] = xyzrgb[i].color.b;
/*
		cout<<xyzrgb[i].coord.x<<", "
		<<xyzrgb[i].coord.y<<", "
		<<xyzrgb[i].coord.z<<", "
		<<xyzrgb[i].color.x<<", "
		<<xyzrgb[i].color.y<<", "
		<<xyzrgb[i].color.z<<"\n";
*/
	}

	glGenVertexArrays(1, &m_VAO);// creates VAO and returns the integer ID
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO); // creates VBO and returns the integer ID
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(
			GL_ARRAY_BUFFER,
			dimension *sizeof(GLfloat),
			vxyz,
			GL_STATIC_DRAW);
	// select 3 coord to points
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GLfloat),
			(GLvoid*) 0
		);
	glEnableVertexAttribArray(0);

	// select next 3 value to color
	glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat))
		);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void init(GLFWwindow *window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 50.0f; // Limit value is far proyection size
	setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
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

	mvMat = glm::translate(
				glm::mat4(1.0f),
				glm::vec3(-cameraX, -cameraY, -cameraZ));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, dimension/6);
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

	GLFWwindow *window = glfwCreateWindow(800, 800, "Lab 06.1.1: Draw cube using planes",	NULL, NULL);
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
