#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

void init(GLFWwindow *window){


    /*--[INICIALIZACION IMGUI]-----------*/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    /*--[INICIALIZACION IMGUI]-----------*/

}

void display(){
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.9,0.8,0.7, 1.0);


    /*--[FRAME IMGUI]-----------*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    /*--[FRAME IMGUI]-----------*/


    /*--[CODIGO IMGUI]-----------*/
    ImGui::Begin("Hello, friend!"); // Aqui inicia una pequeña ventana
    ImGui::Text("Esto es un texto de prueba >:).");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Muestra los FPS de la aplicación
    ImGui::End(); // Aqui termina la ventana ultima en iniciar
    /*--[CODIGO IMGUI]-----------*/


    /*--[RENDER IMGUI]-----------*/
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    /*--[RENDER IMGUI]-----------*/


}

int main(){
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
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
