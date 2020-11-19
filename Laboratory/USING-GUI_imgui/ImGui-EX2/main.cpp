#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include "fun.h"

/*
#define WIDTH 1280
#define HEIGHT 720
*/

#define WIDTH 400
#define HEIGHT 500


/* ImGui */
void display();
void init(GLFWwindow *window);

/* Temas */
void CherryTheme();

int main(){
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    // glfwSetWindowSizeCallback(window, resize_callback);
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


void init(GLFWwindow *window){


    /*--[INICIALIZACION IMGUI]-----------*/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // TEMAS: 
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    // ImGui::StyleColorsLight();
    // CherryTheme();
    
    /*--[INICIALIZACION IMGUI]-----------*/

}


void display(){

    glClear(GL_COLOR_BUFFER_BIT);

    /*--[FRAME IMGUI]-----------*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    ImGui::SetNextWindowPos(ImVec2(0,0));
    /*--[FRAME IMGUI]-----------*/




    /*--[CODIGO IMGUI]-----------*/

    ImGuiStyle& style = ImGui::GetStyle();    
    style.FrameBorderSize = 1.0f;
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    
    ImGui::Begin("Ventana principal", nullptr, window_flags);

    static bool show_ex_sort = false, show_fibo = false;
    static bool edit_apar = false;

    if (ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("Ejemplos")){
            ImGui::MenuItem("Fibonacci", NULL, &show_fibo);
            ImGui::MenuItem("Ordenamiento", NULL, &show_ex_sort);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Apariencia")){
            ImGui::MenuItem("Editar", NULL, &edit_apar);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (show_fibo) {
        if (ImGui::CollapsingHeader("Fibonacci")){
            static int limit = 0;
            ImGui::TextColored(ImVec4(0.5,0.5,0.9,1), "Fibonacci.");
            ImGui::InputInt("Limite", &limit);
            std::vector<int> ans = fibo(limit);
            std::string ans_str = "Serie: ";
            for (int i: ans) ans_str += std::to_string(i) + " ";
            ImGui::Text(ans_str.c_str());
        }
    }

    if (show_ex_sort){
        if (ImGui::CollapsingHeader("Ordenamiento")){
            static std::vector<float> sort_lista;
            static int valor = 0;
            ImGui::TextColored(ImVec4(0.5,0.5,0.9,1), "Ordenamiento.");
            ImGui::Text("Lista: ");
            ImGui::SameLine();
            std::string values_str = "";
            for (int v: sort_lista) values_str += std::to_string(v) + " ";
            ImGui::Text(values_str.c_str());

            ImGui::InputInt("Valor", &valor);

            ImGui::SameLine();

            if (ImGui::Button("Añadir")){
                sort_lista.push_back(valor);
            }

            if (ImGui::Button("Vacear lista")){
                sort_lista.clear();
            }

            ImGui::SameLine();

            if (ImGui::Button("Ordenar")){
                std::sort(sort_lista.begin(), sort_lista.end());
            }
        }
    }
    
    if (edit_apar){
        ImGui::Begin("Editar apariencia");
            static float color_wall[3];
            ImGui::ColorEdit4("Color Fondo", color_wall);
            glClearColor(color_wall[0], color_wall[1], color_wall[2], 1.0);

            static int e = 0;
            ImGui::RadioButton("Dark", &e, 0); ImGui::SameLine();
            ImGui::RadioButton("Classic", &e, 1); ImGui::SameLine();
            ImGui::RadioButton("Ligth", &e, 2);
            ImGui::RadioButton("Cherry", &e, 3);

            if (e == 0) ImGui::StyleColorsDark();
            if (e == 1) ImGui::StyleColorsClassic();
            if (e == 2) ImGui::StyleColorsLight();
            if (e == 3) CherryTheme();
        ImGui::End();
    }
    
    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    /*--[CODIGO IMGUI]-----------*/








    /*--[RENDER IMGUI]-----------*/
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    /*--[RENDER IMGUI]-----------*/
}


void CherryTheme() {
    // cherry colors, 3 intensities
    #define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
    #define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
    #define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
    // backgrounds (@todo: complete with BG_MED, BG_LOW)
    #define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
    // text
    #define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    auto &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = TEXT(0.78f);
    style.Colors[ImGuiCol_TextDisabled]          = TEXT(0.28f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    // style.Colors[ImGuiCol_ChildWindowBg]         = BG( 0.58f);
    style.Colors[ImGuiCol_PopupBg]               = BG( 0.9f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = BG( 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = MED( 0.78f);
    style.Colors[ImGuiCol_FrameBgActive]         = MED( 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = LOW( 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = HI( 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = BG( 0.75f);
    style.Colors[ImGuiCol_MenuBarBg]             = BG( 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]           = BG( 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = MED( 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = MED( 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered]         = MED( 0.86f);
    style.Colors[ImGuiCol_ButtonActive]          = MED( 1.00f);
    style.Colors[ImGuiCol_Header]                = MED( 0.76f);
    style.Colors[ImGuiCol_HeaderHovered]         = MED( 0.86f);
    style.Colors[ImGuiCol_HeaderActive]          = HI( 1.00f);
    // style.Colors[ImGuiCol_Column]                = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    // style.Colors[ImGuiCol_ColumnHovered]         = MED( 0.78f);
    // style.Colors[ImGuiCol_ColumnActive]          = MED( 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = MED( 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]      = MED( 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = TEXT(0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = MED( 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = TEXT(0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = MED( 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = MED( 0.43f);
    // [...]
    style.Colors[ImGuiCol_ModalWindowDarkening]  = BG( 0.73f);

    style.WindowPadding            = ImVec2(6, 4);
    style.WindowRounding           = 0.0f;
    style.FramePadding             = ImVec2(5, 2);
    style.FrameRounding            = 3.0f;
    style.ItemSpacing              = ImVec2(7, 1);
    style.ItemInnerSpacing         = ImVec2(1, 1);
    style.TouchExtraPadding        = ImVec2(0, 0);
    style.IndentSpacing            = 6.0f;
    style.ScrollbarSize            = 12.0f;
    style.ScrollbarRounding        = 16.0f;
    style.GrabMinSize              = 20.0f;
    style.GrabRounding             = 2.0f;

    style.WindowTitleAlign.x = 0.50f;

    style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;
}
