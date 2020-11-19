/* imgui */
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

/* graphics header */
#include <GL\glew.h>
#include <GLFW\glfw3.h>

/* math */
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

/* basic headers */
#include <iostream>

/* own headers */
#include "utils.h"

#define numVAOs 1
#define numVBOs 2

using namespace std;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint mvLoc, projLoc;
int width, heigth;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;
glm::mat4 tMat, rMat;

void setupVertices(void) {
	float vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
	};

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow *window){

    renderingProgram = createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 8.0f;
	cubeLocX = 0.0f;
	cubeLocY = -2.0f;
	cubeLocZ = 0.0f;
	setupVertices();

    /*--[INICIALIZACION IMGUI]-----------*/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    /*--[INICIALIZACION IMGUI]-----------*/

}

void display(GLFWwindow *window){
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.9,0.8,0.7, 1.0);

    /*--[FRAME IMGUI]-----------*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    /*--[FRAME IMGUI]-----------*/


    static float tras_x = 0, tras_y=0, tras_z = 0;
    static float rota_x = 0, rota_y = 0, rota_z = 0;
    static float scale = 1;

    /*--[CODIGO IMGUI]-----------*/
    ImGui::Begin("Transformaciones"); // Aqui inicia una pequeña ventana
    ImGui::Text("Traslaciones");
    ImGui::SliderFloat("tras. x", &tras_x, -10, 10);
    ImGui::SliderFloat("tras. y", &tras_y, -10, 10);
    ImGui::SliderFloat("tras. z", &tras_z, -10, 10);
    ImGui::Text("Rotaciones");
	ImGui::SliderFloat("rota. x", &rota_x, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::SliderFloat("rota. y", &rota_y, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::SliderFloat("rota. z", &rota_z, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::Text("Escalamiento");
	ImGui::SliderFloat("scala", &scale, 0, 4);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Muestra los FPS de la aplicación
    ImGui::End(); // Aqui termina la ventana ultima en iniciar
    /*--[CODIGO IMGUI]-----------*/


    tMat = translate(glm::mat4(1), glm::vec3(tras_x, tras_y, tras_z));
    rMat = glm::rotate(glm::mat4(1), rota_x, glm::vec3(1, 0, 0));
    rMat = glm::rotate(rMat, rota_y, glm::vec3(0, 1, 0));
    rMat = glm::rotate(rMat, rota_z, glm::vec3(0, 0, 1));
    glm::mat4 sMat = glm::scale(glm::mat4(1), glm::vec3(scale, scale, scale));
    mMat = tMat * rMat * sMat;

    glUseProgram(renderingProgram);

    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    glfwGetFramebufferSize(window, &width, &heigth);
    aspect = (float)width / (float)heigth;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    // mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
    mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);  // makes the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associates 0th attribute with buffer
	glEnableVertexAttribArray(0);  // enable the 0th vertex attribute

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glPointSize(10.0); // para ver los vertices del cubo
	glDrawArrays(GL_POINTS, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, 36);



    /*--[RENDER IMGUI]-----------*/
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    /*--[RENDER IMGUI]-----------*/


}

int main(){
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    if (glewInit() != GLEW_OK) return 1;

    init(window);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        display(window);
        glfwSwapBuffers(window);
    }

    /*--[CLEAN IMGUI]-----------*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    /*--[CLEAN IMGUI]-----------*/

}
