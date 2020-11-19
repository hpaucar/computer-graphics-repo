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

GLFWwindow *window;
GLuint vao, vbo;
GLuint program;

float vertices [] = {
    -.5f, -.5f, .0f,
     .5f, -.5f, .0f,
     .0f,  .5f, .0f
};

void init(GLFWwindow *window){

    /*--[INICIALIZACION IMGUI]-----------*/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    /*--[INICIALIZACION IMGUI]-----------*/

    program = createShaderProgram("vshader.glsl", "fshader.glsl" );
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void *) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}

void display(){
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.9,0.8,0.7, 1.0);

    /*--[FRAME IMGUI]-----------*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    /*--[FRAME IMGUI]-----------*/


    static float tras_x = 0, tras_y=0;
    static float rota_x = 0, rota_y = 0;
    static float scale = 1;

    /*--[CODIGO IMGUI]-----------*/
    ImGui::Begin("Transformaciones"); // Aqui inicia una pequeña ventana
    ImGui::Text("Traslaciones");
    ImGui::SliderFloat("tras. x", &tras_x, -1, 1);
    ImGui::SliderFloat("tras. y", &tras_y, -1, 1);
    ImGui::Text("Rotaciones");
	ImGui::SliderFloat("rota. x", &rota_x, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::SliderFloat("rota. y", &rota_y, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::Text("Escalamiento");
	ImGui::SliderFloat("scala", &scale, 0, 4);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Muestra los FPS de la aplicación
    ImGui::End(); // Aqui termina la ventana ultima en iniciar
    /*--[CODIGO IMGUI]-----------*/


    glUseProgram(program);
    glBindVertexArray(vao);
    glm::mat4 trans = translate(glm::mat4(1), glm::vec3(tras_x, tras_y, 0));
    trans = glm::rotate(trans, rota_x, glm::vec3(1, 0, 0));
    trans = glm::rotate(trans, rota_y, glm::vec3(0, 1, 0));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1));
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation_matrix"), 1, false, value_ptr(trans));

    glDrawArrays(GL_TRIANGLES, 0, 3);



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
        display();
        glfwSwapBuffers(window);
    }

    /*--[CLEAN IMGUI]-----------*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    /*--[CLEAN IMGUI]-----------*/

}
