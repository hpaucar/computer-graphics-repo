#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Utils.hpp"

using namespace std;

#define LOG(x) cout<<"\t"<<(x)<<endl;
#define WIDTH 800
#define HEIGHT 800

int MAX_ITER = 90;
double move_x = 0.0f, move_y = 0.0f, move_velocidad = 0.15; 
double zoom = 1; // default: rango de coordenadas (-1, +1) -> (-5, +5)
double sensibilidad_scroll = 1.2;
int op_algoritmo = 4;

GLFWwindow *window;
GLuint VBO;
GLuint VAO;
GLuint sp;

vector<GLdouble> vertices = {
     0.95f,  0.95f, 0.0f, 
     0.95f, -0.95f, 0.0f,
    -0.95f, -0.95f, 0.0f,
    -0.95f,  0.95f, 0.0f
};

void init(GLFWwindow* window){
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    sp = Utils::createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(sp);

    //glm::vec2 u_resolution = glm::vec2(WIDTH, HEIGHT);
    glUniform2d(glGetUniformLocation(sp, "u_resolution"), WIDTH, HEIGHT);
    glUniform1i(glGetUniformLocation(sp, "MAX_ITER"), MAX_ITER);
    glUniform1d(glGetUniformLocation(sp, "zoom"), zoom);
    glUniform1i(glGetUniformLocation(sp, "op"), op_algoritmo);
}
    
void display(GLFWwindow *window, double currentTime){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Mandelbrot Set");
    if(ImGui::InputInt("Num. iteraciones", &MAX_ITER)){
        glUniform1i(glGetUniformLocation(sp, "MAX_ITER"), MAX_ITER);
    }
    
    ImGui::SliderFloat("Vel. movimiento", (float *) &move_velocidad, 1, 3);
    
    if(ImGui::RadioButton("Escape Time", &op_algoritmo, 1)){
        glUniform1i(glGetUniformLocation(sp, "op"), op_algoritmo);
        cout<<"OP::"<<op_algoritmo<<endl;
    }
    if(ImGui::RadioButton("Escape Time Optimizado", &op_algoritmo, 2)){
        glUniform1i(glGetUniformLocation(sp, "op"), op_algoritmo);
        cout<<"OP::"<<op_algoritmo<<endl;
    }
    if(ImGui::RadioButton("ET con mapeado de color", &op_algoritmo, 3)){
        glUniform1i(glGetUniformLocation(sp, "op"), op_algoritmo);
        cout<<"OP::"<<op_algoritmo<<endl;
    }
    if(ImGui::RadioButton("ET Optimizado con mapeado de color", &op_algoritmo, 4)){
        glUniform1i(glGetUniformLocation(sp, "op"), op_algoritmo);
        cout<<"OP::"<<op_algoritmo<<endl;
    } 
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Cant. zoom: %.3f x", log2f(zoom)/2);
    ImGui::Separator();
    ImGui::Text("Rendimiento %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::End();

    glClearColor(0.3f, 0.6f, 0.9f, 0.95f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_QUADS, 0, vertices.size()/3);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        move_x += move_velocidad/zoom;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        move_x -= move_velocidad/zoom;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        move_y -= move_velocidad/zoom;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        move_y += move_velocidad/zoom;
    glUniform1d(glGetUniformLocation(sp, "move_x"), move_x);
    glUniform1d(glGetUniformLocation(sp, "move_y"), move_y);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{   
    if (yoffset == 0) return;
    double old_zoom = zoom;
    zoom = yoffset<0? zoom/sensibilidad_scroll:zoom*sensibilidad_scroll;
    double mouse_x, mouse_y; glfwGetCursorPos(window, &mouse_x, &mouse_y);
    mouse_x = mouse_x/(WIDTH/2.0) - 1.0;
    mouse_y = (mouse_y/(HEIGHT/2.0) - 1.0) * -1;
    move_x += mouse_x/zoom - mouse_x/old_zoom;
    move_y += mouse_y/zoom - mouse_y/old_zoom;
    glUniform1d(glGetUniformLocation(sp, "move_x"), move_x);
    glUniform1d(glGetUniformLocation(sp, "move_y"), move_y);
    glUniform1d(glGetUniformLocation(sp, "zoom"), zoom);
}

int main(){
    if(!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Set - Shader double", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
    cout<<"GL_VERSION: "<<glGetString(GL_VERSION)<<endl;
    glfwSwapInterval(0);

    init(window);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteShader(sp);
    glDeleteVertexArrays(1, &VAO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
