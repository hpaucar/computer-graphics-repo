/*
 *	S: trasladar a la Izquierda
 *	D: Trasladar a la Derecha
 *  I: Hacer Zoom In
 *  J: Hacer Zoom Out.
 * */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"

#include <string>

using namespace std;

GLuint m_VAO;

GLuint vbo_surface;
GLuint vbo_color;
GLuint vbo_cp;

GLint uniform_mvp;

GLuint program;

GLint attribute_coord3d;
GLint attribute_color;

GLfloat* points;
GLfloat* colors;

int numPoints;

int screen_width = 800, screen_height = 800;

float dx = 0.0;
float scale = 1.0;


GLfloat cp[4][4][3] = {
    {{0.0, 0.0, 1.0},
     {0.0, 0.0, 0.66},
     {0.0, 0.0, 0.33},
     {0.0, 0.0, 0.0}},

    {{0.33, 0.0, 1.0},
     {0.33, 1.5, 0.66},
     {0.33, 1.5, 0.33},
     {0.33, 0.0, 0.0}},

    {{0.66, 0.0, 1.0},
     {0.66, 1.5, 0.66},
     {0.66, 1.5, 0.33},
     {0.66, 0.0, 0.0}},

    {{1.0, 0.0, 1.0},
     {1.0, 0.0, 0.66},
     {1.0, 0.0, 0.33},
     {1.0, 0.0, 0.0}}
};

int number_of_steps = 150;

int factorial[4] = {1, 1, 2, 6};

float binomial(int n, int i) {
    return factorial[n] / (factorial[i] * factorial[n - i]) * 1.0;
}

float bernstein(int m, int i, float t) {
    return binomial(m, i) * powf(t, i) * powf(1 - t, m - i);
}

void print(std::string s) {
    std::cout << s << std::endl;
}

vector<GLfloat> getControlPoints(){
	vector<GLfloat> temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 3; k++)
				temp.push_back(cp[i][j][k]);
	return temp;
}

bool init_resources() {
    float u = 0.0, v = 0.0;
    float x, y, z;
    float B_i, B_j;

	std::vector<float> ps;

	for (int i = 0; i <= number_of_steps; i++) { // recorrer el parámetro u
		v = 0.0;
		for (int j = 0; j <= number_of_steps; j++) { // recorrer el parámetro v
			x = y = z = 0.0;
			// para la sumatoria anidada en la formula de Bezier
			for (int k = 0; k < 4; k++) { // sumatoria en formula de Bezier
				B_i = bernstein(3, k, u);
				for (int l = 0; l < 4; l++) { // sumatoria en formula de Bezier
					B_j = bernstein(3, l, v);
					x += B_i * B_j * cp[k][l][0];
					y += B_i * B_j * cp[k][l][1];
					z += B_i * B_j * cp[k][l][2];
				}
			}
			ps.push_back(x);
			ps.push_back(y);
			ps.push_back(z);
			v += 1.0 / number_of_steps;
		}
		u += 1.0 / number_of_steps;
	}

	numPoints = ps.size() / 3;

    points = new GLfloat[ps.size()];
    colors = new GLfloat[ps.size()];
    for (unsigned int i = 0; i < ps.size(); i++) {
        points[i] = ps[i];
    }
    for (unsigned int i = 0; i < ps.size() / 3; i++) {
        colors[3 * i] = 0.0;
        colors[3 * i + 1] = 1.0;
        colors[3 * i + 2] = 0.0;
    }

	// Create Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

    glGenBuffers(1, &vbo_surface);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_surface);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ps.size(), points, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ps.size(), colors, GL_STATIC_DRAW);

    vector<GLfloat> controlPoints = getControlPoints();
    glGenBuffers(1, &vbo_cp);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cp);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * controlPoints.size(), (void*)&controlPoints[0], GL_STATIC_DRAW);

    program = Utils::createShaderProgram("src/basic3.v.glsl", "src/basic3.f.glsl");

    attribute_coord3d = glGetAttribLocation(program, "coord3d");
    if(attribute_coord3d == -1){
        std::cout << "No se puede asociar el atributo coord3d" << std::endl;
        return false;
    }

    attribute_color = glGetAttribLocation(program, "color");
    if(attribute_color == -1){
        std::cout << "No se puede asociar el atributo color" << std::endl;
        return false;
    }

    uniform_mvp = glGetUniformLocation(program, "mvp");
    if(uniform_mvp == -1){
        std::cout << "No se puede asociar el uniform mvp" << std::endl;
        return false;
    }
    getControlPoints();
    return true;
}

void keyboard(GLFWwindow* window){
	// Move RIGHT
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            dx+= 0.0005;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            dx+= -0.0005;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            scale = scale * 1.002;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            scale = scale * 0.998;
}

void onDisplay(){

	glUseProgram(program);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);	// Habilita este atributo Layout 0

    //Creamos matrices de modelo, vista y proyeccion
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(dx, 0, 0));
    model = glm::translate(model, glm::vec3(scale, scale, scale));

    glm::mat4 view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    glm::mat4 mvp = projection * view * model ;

    //Enviamos la matriz que debe ser usada para cada vertice
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    // enlazar variable de shader de coordenadas con el buffer de data de vertices y especificar el formato en el que se leerá
    glEnableVertexAttribArray(attribute_coord3d);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_surface);

    glVertexAttribPointer(
        attribute_coord3d,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,0
    );

    // enlazar variable de shader de colores con el buffer de colores y especificar el formato en el que se leerá
    glEnableVertexAttribArray(attribute_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glVertexAttribPointer(
        attribute_color
        ,3
        ,GL_FLOAT
        ,GL_FALSE
        ,0,0
    );

    glBindBuffer(GL_ARRAY_BUFFER, vbo_surface);
    
    //glDrawArrays(GL_POINTS, 0, numPoints);
    glDrawArrays(GL_LINE_STRIP, 0, numPoints);
    

	// Draw Control Points
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cp);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, 16);


    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_color);
}

void onReshape(int w, int h){
    screen_width = w;
    screen_height = h;

    glViewport(0,0,screen_width, screen_height);
}

void free_resources(){
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_surface);
    glDeleteBuffers(1, &vbo_color);

    delete[] points;
    delete[] colors;
}

int main(int argc, char* argv[]){
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	GLFWwindow *window = glfwCreateWindow(800, 800, "Lab: Draw Bezier Curve",
	NULL, NULL);
	glfwMakeContextCurrent(window);

    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK){
        std::cout << "Error inicializando GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!GLEW_VERSION_2_0){
        std::cout << "Tu tarjeta grafica no soporta OpenGL 2.0" << std::endl;
        exit(EXIT_FAILURE);
    }

    init_resources();
    while (!glfwWindowShouldClose(window)) {
    	onDisplay();
    	//onReshape();
    	keyboard(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free_resources();
    exit(EXIT_SUCCESS);
}
