#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Utils.hpp"

using namespace std;

void initialice_n_context();
void setup();
void init(GLFWwindow *window);
void display(GLFWwindow *window, double currentTime);

const int WIDTH = 800;
const int HEIGTH = 600;
int n_itr = 100;
unsigned int shaderProgram;
GLFWwindow *window;

float vertices[] = {  // (ventana completa)
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
 
     1.0f,  1.0f,
    -1.0f,  1.0f,
    -1.0f, -1.0f
};

int main() {
    initialice_n_context();
    init(window);
    while(!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void display(GLFWwindow *window, double currentTime) {
        glUniform1f(glGetUniformLocation(shaderProgram, "u_time"), (float)currentTime);
        glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
}

void init(GLFWwindow *window) {
    setup();
    glUseProgram(shaderProgram);

}

void initialice_n_context(){
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(WIDTH, HEIGTH, "Increible fragment shader!!", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
    cout<<"GL_VERSION: "<<glGetString(GL_VERSION)<<endl;
}

void setup(){
    // setup objects
    unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    // setup shaders
    shaderProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
}
