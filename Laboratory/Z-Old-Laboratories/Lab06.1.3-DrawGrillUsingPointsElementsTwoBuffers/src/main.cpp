//==============================================================================================
// Name        : Display Grill using GL_LINES primitives with events
// Professor   : Herminio Paucar
// Version     :
// Description : El código utiliza todos los puntos de la grilla, los puntos son considerados elementos
//				y son diseñados usando 2 Buffers.
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

	// Create Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &m_VBO);
	int length = 100;
	std::vector<glm::vec3> grillxyz = MyFunctions::getGrillPoints(glm::vec3(0, 0, 0), 10, length, "xz");
	dimension = grillxyz.size()*6;

	float vxyz[dimension];
	for(unsigned int i=0; i< grillxyz.size(); i++){
		vxyz[6*i+0] = grillxyz[i].x;
		vxyz[6*i+1] = grillxyz[i].y;
		vxyz[6*i+2] = grillxyz[i].z;
		vxyz[6*i+3] = 0;
		vxyz[6*i+4] = 1;
		vxyz[6*i+5] = 0;

	/*	cout<<"P"<<i<<": ("
			<<grillxyz[i].x<<", "
			<<grillxyz[i].y<<", "
			<<grillxyz[i].z<<")\n"; */
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			dimension * sizeof(GLfloat),	// tamanho do buffer
			vxyz,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização

    // Create an element array
    GLuint m_EBO;
    glGenBuffers(1, &m_EBO);
    int numQuads = glm::pow(2*length,2);
    //Number of points to rectangle using GL_LINES is 4 lines (8 points)
    eleDim = numQuads*8;
    GLuint elements[eleDim];
    glm::vec4 temp;
    int cont = 0;
    // Ordenamos todos los puntos de la grilla que pertenecen a un cuadrado.
    for(unsigned int i=0; i < (grillxyz.size()-(2*length+1)); i++){
    	if((i+1)%(2*length+1)!=0){
    		temp.x = i;
    		temp.y = i + 1;
    		temp.z = length * 2 + (i + 1);
    		temp.w = temp.z + 1;

			elements[8 * cont + 0] = temp.x; // Line Top
			elements[8 * cont + 1] = temp.y;
			elements[8 * cont + 2] = temp.y; // Line Right
			elements[8 * cont + 3] = temp.w;
			elements[8 * cont + 4] = temp.w; // Line Bottom
			elements[8 * cont + 5] = temp.z;
			elements[8 * cont + 6] = temp.z; // Line Left
			elements[8 * cont + 7] = temp.x;

    		cont++;
    	}
    }

/* Operation to draw grill using 2 triangles to one Quad
 *
    eleDim = numQuads*6;
    GLuint elements[eleDim];
    glm::vec4 temp;
    int cont = 0;
    // Ordenamos todos los puntos de la grilla que pertenecen a un cuadrado.
    for(unsigned int i=0; i < (grillxyz.size()-(2*length+1)); i++){
    	if((i+1)%(2*length+1)!=0){
    		temp.x = i;
    		temp.y = i + 1;
    		temp.z = length * 2 + (i + 1);
    		temp.w = temp.z + 1;

    		elements[6*cont+0] = temp.x;
    		elements[6*cont+1] = temp.y;
    		elements[6*cont+2] = temp.z;
    		elements[6*cont+3] = temp.y;
    		elements[6*cont+4] = temp.w;
    		elements[6*cont+5] = temp.z;

    		cont++;
    	}
    }
 */

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(elements),
			elements,
			GL_STATIC_DRAW);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(renderingProgram, "iPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(
			posAttrib,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GLfloat),
			0);

	GLint colAttrib = glGetAttribLocation(renderingProgram, "iColor");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(
			colAttrib,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GLfloat),
			(void*) (3 * sizeof(GLfloat)));
}

void init(GLFWwindow *window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
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
	//glDrawArrays(GL_POINTS, 0, dimension/6);
	//glDrawArrays(GL_TRIANGLES, 0, dimension/6);
    glDrawElements(GL_LINES, eleDim, GL_UNSIGNED_INT, 0);
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
