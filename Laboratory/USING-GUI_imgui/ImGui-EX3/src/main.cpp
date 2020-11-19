#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"
#include <stdio.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\SOIL2.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>


#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>

#include "Utils.h"

#define numVAOs 1
#define numVBOs 2
#define MATIDE glm::mat4(1.0)
#define VECX glm::vec3(1,0,0)
#define VECY glm::vec3(0,1,0)
#define VECZ glm::vec3(0,0,1)
#define VECNull glm::vec3(0,0,0)

using namespace std;

float cameraX = 2.0f, cameraY = 0.0f, cameraZ = 1.0f;
GLuint renderingProgram;
GLuint brickTexture;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

glm::mat4 pMat, vMat, mMat, mvMat;

bool fast = false, piramide = false;


void setupVertices(void) {

    float pyrTextCoords[36] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // top and rigth faces
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back andl eft faces
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f  // base triangles
    };

    float pyramidPositions[54] = {
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, // front face
         1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, // right face
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, // back face
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, // left face
        -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, // base – left front
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f // base – right back
    };

    // create VAO and VBOs & push data in VBO
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTextCoords), pyrTextCoords, GL_STATIC_DRAW);

}

void init(GLFWwindow *window) {
    setupVertices();
    renderingProgram = Utils::createShaderProgram("src\\vertShader.glsl", "src\\fragShader.glsl");


    brickTexture = Utils::loadTexture("brick1.jpg");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 5.0f; // camera for VIEW matrix

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");


    int width, heigth;
    glfwGetFramebufferSize(window, &width, &heigth);
    pMat = glm::perspective(1.0472f, (float)(width/heigth), 0.1f, 1000.0f);
    vMat = glm::translate(MATIDE, glm::vec3(-cameraX, -cameraY, -cameraZ));
}

void drawPiramide(double currentTime){
    glUseProgram(renderingProgram);
	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));

    stack<glm::mat4> mvStack;
    mvStack.push(vMat);
    mvStack.push(mvStack.top());

    glm::mat4 traslacion = glm::translate(MATIDE, glm::vec3(sin(currentTime), 0, 0));
    glm::mat4 rotacion = glm::rotate(MATIDE, (float)currentTime, VECX);
    glm::mat4 model =  rotacion * traslacion;

    glm::mat4 model_view = vMat * model ;
    // ------------------- ready to draw SUN -----------------------------
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(model_view));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);  // makes the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associates 0th attribute with buffer
	glEnableVertexAttribArray(0);  // enable the 0th vertex attribute

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 18);
}

void display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Proyecto de Computacion Gráfica");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("Escoja la velocidad:");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Mover rapido?", &fast);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Dibujar piramide?", &piramide);      // Edit bools storing our window open/close state

    ImGui::End();

    if (fast)
        currentTime *= 10;
    else
        currentTime *= 0.5;

    if (piramide)
        drawPiramide(currentTime);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

int main() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow * window = glfwCreateWindow(600, 600, R"( Proyecto-CG )", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
    glfwSwapInterval(1);

    init(window);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
