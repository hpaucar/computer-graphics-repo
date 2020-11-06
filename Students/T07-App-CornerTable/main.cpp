#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include "Utils.hpp"
#include "CornerTable.h"
#include "min_path.h"

#define numVAOs 1
#define numVBOs 1
#define numEBOs 2

#define PI 3.14
#define DIJKSTRA 1
#define BFS 2
double normalize = 1;

using namespace std;
using namespace std::chrono;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numEBOs];

float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 1.0f;
float objLocX = -0.3f, objLocY = 0.0f, objLocZ = 0.0f;

int width, heigth;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

CornerTable *CT;
string offFilePath = "offFiles/torus.off";
vector<unsigned int> path_indices;
string descFile = "";
duration<double> diff;
int o, d;
int op_model;
int op_algor;

/*------------------------------ [CONTROLES] ---------------------------*/
bool dragging = false;
double offsetX = 0.0;
double offsetY = 0.0;
double oldX = 0, oldY = 0;
double zoom = 1;
glm::mat4 transf = glm::mat4(1.0);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        glfwGetCursorPos(window, &oldX, &oldY);
        dragging = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        dragging = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    if (dragging){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        offsetX = (xpos - oldX)/zoom;
        offsetY = (ypos - oldY)/zoom;

        oldX = xpos;
        oldY = ypos;

        float angleRotX = (PI/800) * offsetY;
        float angleRotY = (PI/800) * offsetX;

        transf = glm::rotate(transf, angleRotX, glm::vec3(1.0, 0.0, 0.0));
        transf = glm::rotate(transf, angleRotY, glm::vec3(0.0, 1.0, 0.0));
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    double sensibilidad = 1.09;
    if(yoffset != 0){
        zoom = yoffset>0? sensibilidad:1/sensibilidad;
        transf = glm::scale(transf, glm::vec3(zoom, zoom, zoom));
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        transf = glm::mat4(1.0f);

    double vel = 0.15/zoom;
    if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
            transf = glm::translate(transf, glm::vec3(-vel, 0.0, 0.0));

    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
            transf = glm::translate(transf, glm::vec3(+vel, 0.0, 0.0));

    if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
            transf = glm::translate(transf, glm::vec3(0.0, vel, 0.0));

    if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS)
			transf = glm::translate(transf, glm::vec3(0.0, -vel, 0.0));
}
/*------------------------------------------------------------------*/

/*------------------------------ [IMGUI] -------------------------------*/
    bool
    ALL_TRIANG_BORDER = false,
    PRINT_PATH = true,
    PRINT_PATH_TRIANG_BORDER = true,
    PRINT_VERT = false;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00);
    ImVec4 surf_color = ImVec4(62.f/255.f, 60.f/255.f, 165.0f/255.f, 1.00f);
    ImVec4 path_color = ImVec4(217.f/255.f, 255.f/255.f, 0.00f, 1.00f);

/*---------- -------------------------------------------------------*/


void readMeshFiles() {
    ifstream fin("offFiles/"+offFilePath);
    cout<<"[LOG]\tFichero OFF cargado"<<endl;

    string name;
    int npoint, ntriangle, var3;
    int var0;

    fin>>name;
    fin>>npoint>>ntriangle>>var3;
    cout<<"[LOG]\tCantidades leidas"<<endl;

    double* vertexList = new double[3*npoint];
    for (int i = 0; i < npoint; ++i){
        fin>>vertexList[3*i]>>vertexList[3*i+1]>>vertexList[3*i+2];
        normalize = max(normalize, abs(vertexList[3*i]));
        normalize = max(normalize, abs(vertexList[3*i+1]));
        normalize = max(normalize, abs(vertexList[3*i+2]));
    }
    cout<<"[LOG]\tVertices leidos"<<endl;

    const CornerType numberTriangles = ntriangle;
    const CornerType numberVertices = npoint;

    CornerType* triangleList = new CornerType[3*ntriangle];
    for (int i = 0; i <ntriangle; ++i)
        fin>>var0>>triangleList[i*3]>>triangleList[i*3+1]>>triangleList[i*3+2];
    cout<<"[LOG]\tIndices triangulos leidos"<<endl;

    int numberCoordinatesByVertex = 3;

    CornerTable *ct = new CornerTable(triangleList, vertexList,
        numberTriangles, numberVertices, numberCoordinatesByVertex);
    cout<<"[LOG]\tCorner Table creada"<<endl<<endl;

    descFile +="_________________________[OFF]_________________________ \n";
    descFile +="\t\t (*) File: " + offFilePath + "\n";
    descFile +="\t\t (*) Num vertices: " + to_string(npoint) + "\n";
    descFile +="\t\t (*) Num triangulos: " + to_string(ntriangle) + "\n";
    descFile +="\t\t (*) Num colores: " + to_string(var3) + "\n";
    descFile +="_________________________[OFF]_________________________ \n";

    cout<<descFile;
    descFile = "";
    descFile +="______________[OFF]______________ \n";
    descFile +="\t (*) File: " + offFilePath + "\n";
    descFile +="\t (*) Num vertices: " + to_string(npoint) + "\n";
    descFile +="\t (*) Num triangulos: " + to_string(ntriangle) + "\n";
    descFile +="\t (*) Num colores: " + to_string(var3) + "\n";
    descFile +="______________[OFF]______________ \n";

    CT = ct;
    fin.close();
}

void buildPath(int o, int d){
    vector<unsigned int> path;
    const CornerType *trianglesPositions = CT->getTriangleList();

    auto start = high_resolution_clock::now();

    if (op_algor == DIJKSTRA) {
    	cout<<"[LOG]\t Algoritmo de Dijkstra"<<endl;
    	MinPathDijkstra(path, CT, o, d);
    }
    if (op_algor == BFS){
    	cout<<"[LOG]\t Algoritmo BFS"<<endl;
    	MinPathBFS(path, CT, o, d);
    }

    auto stop = high_resolution_clock::now();
    diff = duration_cast<microseconds>(stop - start);

    for(unsigned int i =0; i<path.size(); i++){
        path_indices.push_back(trianglesPositions[path[i]*3]);
        path_indices.push_back(trianglesPositions[path[i]*3 + 1]);
        path_indices.push_back(trianglesPositions[path[i]*3 + 2]);
    }
}

void setupVertices(void) {

    double* vertexPositions = CT->getAttributes();
    const CornerType *trianglesPositions = CT->getTriangleList();

    for (unsigned int i=0; i<CT->getNumberVertices()*3; i++) // normalizar coordenadas
        vertexPositions[i] *= 1/(normalize*2);

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, CT->getNumberVertices()*3*sizeof(double), &vertexPositions[0], GL_STATIC_DRAW);

    glGenBuffers(numEBOs, ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CT->getNumTriangles()*3*sizeof(CornerType), &trianglesPositions[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, path_indices.size()*3*sizeof(CornerType), &path_indices[0], GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    glUseProgram(renderingProgram);

    buildPath(o, d);
    setupVertices();

    glfwGetFramebufferSize(window, &width, &heigth);
    aspect = (float)width / (float)heigth;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
}

void display(GLFWwindow *window, double currentTime) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Camino mas corto en CornerTable - GUI");
    ImGui::Text("PINTAR:");
    ImGui::Checkbox("Camino minimo encontrado", &PRINT_PATH);
    ImGui::Checkbox("Bordes de triangulos en el Camino minimo", &PRINT_PATH_TRIANG_BORDER);
    ImGui::Checkbox("Todos los vertices", &PRINT_VERT);
    ImGui::Checkbox("Bordes de todos los triangulos", &ALL_TRIANG_BORDER);
    ImGui::Separator();
    ImGui::ColorEdit3("Color de superficie", (float*)&surf_color);
    ImGui::ColorEdit3("Color de camino", (float*)&path_color);
    ImGui::ColorEdit3("Color de fondo", (float*)&clear_color);


    if (ImGui::CollapsingHeader("Transformaciones")){

        ImGui::Text("Controles:");
        ImGui::Indent();
			ImGui::Text("ROTACIONES:");
			string t;
			t  = "EJE X: Click derecho y arrastre\n";
			t += "el mouse en forma vertical \n";
			ImGui::BulletText(t.c_str());
			t  = "EJE Y: Click derecho y arrastre\n";
			t += "el mouse en forma horizontal\n";
			ImGui::BulletText(t.c_str());
			ImGui::BulletText("Presione 'D' para deshacer todas\nlas rotaciones");
        ImGui::Unindent();

        ImGui::Indent();
			ImGui::Text("TRASLACIONES:");
			t = "IZQUIERDA: Flecha Izqui. o 'A'\n";
			ImGui::BulletText(t.c_str());
			t = "DERECHA: Flecha Derecha. o 'D'\n";
			ImGui::BulletText(t.c_str());
			t = "ARRIBA: Flecha Arriba o 'W'\n";
			ImGui::BulletText(t.c_str());
			t = "ABAJO: Flecha Abajo o 'D'\n";
			ImGui::BulletText(t.c_str());
        ImGui::Unindent();

        ImGui::Indent();
			ImGui::Text("ZOOM in/out:");
			ImGui::BulletText("Use el scroll de su mouse");
        ImGui::Unindent();
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Informacion (OFF file)")){
        ImGui::Text(descFile.c_str());
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Autores")){
    	ImGui::Text("\t\t\t\t\t\t  UNMSM"  );
    	ImGui::Text("\t\t\t\t\t    FISI 2020");
    	ImGui::Indent();
			ImGui::Text("\t\t\t\tCOMPUTACION GRAFICA");
			ImGui::Text("Profesor");
			ImGui::BulletText("Herminio Paucar Curasma (@hpaucar)");
			ImGui::Text("Alumnos");
			ImGui::BulletText("Galarza Arevalo, Jonathan");
			ImGui::BulletText("Hidalgo Diaz, Sebastian Eduardo");
			ImGui::BulletText("Moquillaza Alcarraz, Santiago Yovany");
			ImGui::BulletText("Ramos Paredes, Roger Anthony");
			ImGui::BulletText("Rios Jaimes, Jhonel Enrique");
			ImGui::BulletText("Villarreal Doroteo, Omar");
			ImGui::Separator();
		ImGui::Unindent();
    }

    ImGui::Separator();
    ImGui::Text("Promedio de aplicacion %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::End();

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
    mMat *= transf;
    mvMat = vMat * mMat;
	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(mvMat));

    // ------------------ [VBO VERTICES - Bind vertices] ---------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glm::vec3 color1;

    // ------------------ [EBO ALL Triangles - Bind EBO and PAINT MESH] ------------------
    color1 = glm::vec3(surf_color.x, surf_color.y, surf_color.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, value_ptr(color1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glDrawElements(
        GL_TRIANGLES,
        CT->getNumTriangles()*3,
        GL_UNSIGNED_INT,
        0
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (ALL_TRIANG_BORDER){
        // ------------------ [PAINT LINES (ALL TRIANGLES BORDER)] ----------------------
        color1 = glm::vec3(1.0, 1.0, 1.0);
        glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, value_ptr(color1));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

        for(unsigned int i=0; i<CT->getNumTriangles() ; i++){
            glDrawElements(
                GL_LINE_LOOP,
                3,
                GL_UNSIGNED_INT,
                (void*)(i*3*sizeof(GL_UNSIGNED_INT))
            );
        }
    }

    if (PRINT_PATH){
        // ------------------ [EBO PATH Triangles - Bind EBO and PAINT PATH] ------------
        color1 = glm::vec3(path_color.x, path_color.y, path_color.z);
        glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, value_ptr(color1));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
        glDrawElements(
            GL_TRIANGLES,
            path_indices.size(),
            GL_UNSIGNED_INT,
            0
        );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    if (PRINT_PATH_TRIANG_BORDER){
        // ------------------ [PAINT LINES (PATH TRIANGLES BORDER)] --------------------
        color1 = glm::vec3(1.0, 0.0, 0.0);
        glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, value_ptr(color1));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);

        for(int i=0; (unsigned int)i<path_indices.size()/3 ; i++){
            glDrawElements(
                GL_LINE_LOOP,
                3,
                GL_UNSIGNED_INT,
                (void*)(i*3*sizeof(GL_UNSIGNED_INT))
            );
        }
    }

    if (PRINT_VERT){
        // ------------------ [PAINT POINTS (TRIANGLES VERTICES)] -----------------------
        color1 = glm::vec3(0.0, 0.0, 0.0);
        glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, value_ptr(color1));
        glPointSize(4.0);
        glDrawArrays(GL_POINTS, 0, CT->getNumberVertices());
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

vector<string> offFiles = {{
    "3dsphere.off",
    "3dtorus.off",
    "2dgrid.off",
    "mesh2.off",
    "mesh3.off",
    "mesh4.off",
    "mesh5.off",
    "mesh6.off",
    "mesh7.off",
    "mesh8.off",
    "mesh9.off",
    "mesh10.off",
    "mesh11.off",
    "mesh12.off",
    "mesh13.off",
    "mesh14.off",
    "mesh15.off",
    "mesh16.off",
    "mesh17.off",
    "mesh18.off",
    "mesh19.off",
    "mesh20.off",
    "mesh21.off",
    "mesh22.off",
    "mesh23.off",
    "mesh24.off",
    "mesh25.off",
    "mesh26.off",
}};

int main() {

    system("cls");
    cout<<"_________________[Modelos disponibles]_________________"<<endl;
    for(unsigned int i=0; i<offFiles.size(); i++){
        if (i==0) cout<<"\t[3D Models]"<<endl;
        if (i==2) cout<<"\t[2D Models]"<<endl;
        cout<<"\t"<<(i+1)<<". "<<offFiles[i]<<endl;
    }
    cout<<"_______________________________________________________"<<endl;
    cout<<"Opcion: "; cin>>op_model;

    if (op_model == 1) normalize = 1.2;
    if (op_model == 3) normalize = 0.5*0.5*0.25;

    offFilePath = offFiles[op_model-1];
    readMeshFiles();
    cout<<endl;
    cout<<"________________[Algoritmos disponibles]_______________"<<endl;
    cout<<"\t 1. Dijkstra - Tiempo: O(n^2)"<<endl;
    cout<<"\t 2. Breadth First Search (BFS) - Tiempo: O(n)"<<endl;
    cout<<"_______________________________________________________"<<endl;
    cout<<"Opcion: "; cin>>op_algor;
    cout<<"_______________[Calculo del Camino Minimo]_____________"<<endl;
    cout<<endl;
    cout<<"\t(*) Ingrese el triangulo de origen (0-"<<CT->getNumTriangles()-1<<"): "; cin>>o;
    cout<<"\t(*) Ingrese el triangulo de destino (0-"<<CT->getNumTriangles()-1<<"): "; cin>>d;
    cout<<"_______________________________________________________"<<endl;

    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow * window = glfwCreateWindow(1400, 800, "CG-T07-CornerTable", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
    cout<<"GL_VERSION: "<<glGetString(GL_VERSION)<<endl;
    glfwSwapInterval(0);

    init(window);
    cout<<"\t(*) Tiempo total: "<<diff.count()<<" seg."<<endl;
    cout << "_______________________________________________________"<<endl;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(numVBOs, vbo);
    glDeleteBuffers(numEBOs, ebo);
    glDeleteShader(renderingProgram);
    glDeleteVertexArrays(numVAOs, vao);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
/*
g++ -o2 -Wall main.cpp -o main -L..\..\dependencies -lglew32s -lglu32 -lopengl32 -lglfw3 -mwindows -mconsole
*/
