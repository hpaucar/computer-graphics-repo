#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_glfw.h"
#include "..\imgui\imgui_impl_opengl3.h"

#include "..\irrklang\irrKlang.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Utils.hpp"
#include "curvas.h"

#define LOG(x) cout << "[LOG]\t" << (x) << endl
#define numVAOs 1
#define numVBOs 14
/*
 * VBO 0: Puntos de control
 * VBO 1: Contorno
 * VBO 2: Barbilla
 * VBO 3: Ojo Izquierdo
 * VBO 4: Ojo Derecho
 * VBO 5: Nariz
 * VBO 6: Detalle-arco-boca
 * VBO 7: Oreja
 * VBO 8: Detalle-arco1-oreja
 * VBO 9: Detalle-arco2-oreja
 * VBO 10: Iris izquierda
 * VBO 11: Iris derecha
 * VBO 12: Detalle-ceja
 * VBO 13: Detalle-labio
 * */
#define PI 3.14
#define WIDTH 800
#define HEIGHT 800

using namespace std;
using namespace irrklang;

// for audio
ISoundEngine* engine_audio;

/* VARIABLES GLOBALES */
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 1.0f;
float objLocX = 0.0f, objLocY = -0.4f, objLocZ = 0.0f;
double zoom = 1;
glm::mat4 pMat, vMat, mMat, mvMat;
glm::mat4 transf = glm::mat4(1.0);
int cantPelosBart = 9;
float alturaPelos = 3.5f;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
vector<double> contorno;
vector<double> barbilla;
vector<double> ojo_izquierdo;
vector<double> ojo_derecho;
vector<double> nariz;
vector<double> det_1_boca;
vector<double> oreja;
vector<double> det_1_oreja;
vector<double> det_2_oreja;
vector<double> iris_izq;
vector<double> iris_der;
vector<double> det_ceja;
vector<double> det_labio;

vector<glm::vec2> p_control{
    /*0*/ glm::vec2{-0.39, -0.0975},
    /*1*/ glm::vec2{-0.3625, 0.2675},
    /*2*/ glm::vec2{-0.4875, 0.63},
    /*3*/ glm::vec2{-0.23, 0.925},
    /*4*/ glm::vec2{0.105, 0.72},
    /*5*/ glm::vec2{0.0825, 0.19},
    /*6*/ glm::vec2{0.1475, 0.0475},
    /*7*/ glm::vec2{0.015, -0.04},
    /*8*/ glm::vec2{-0.2475, 0.0425},
    /*9*/ glm::vec2{-0.0475, 0.0025},
    /*10*/ glm::vec2{-0.07, -0.0425},
    /*11*/ glm::vec2{-0.155, -0.0375},
    /*12*/ glm::vec2{-0.1575, -0.0775},
    /*13*/ glm::vec2{-0.1675, -0.1075},
    /*14*/ glm::vec2{-0.0275, 0.33},
    /*15*/ glm::vec2{-0.0325, 0.54},
    /*16*/ glm::vec2{-0.3275, 0.5075},
    /*17*/ glm::vec2{-0.28, 0.2925},
    /*18*/ glm::vec2{-0.235, 0.175},
    /*19*/ glm::vec2{-0.04, 0.175},
    /*20*/ glm::vec2{-0.0675, 0.3275},
    /*21*/ glm::vec2{-0.08, 0.4825},
    /*22*/ glm::vec2{0.115, 0.525},
    /*23*/ glm::vec2{0.16, 0.365},
    /*24*/ glm::vec2{0.185, 0.1875},
    /*25*/ glm::vec2{-0.0425, 0.1925},
    /*26*/ glm::vec2{-0.0375, 0.265},
    /*27*/ glm::vec2{0.0525, 0.255},
    /*28*/ glm::vec2{0.095, 0.2575},
    /*29*/ glm::vec2{0.135, 0.245},
    /*30*/ glm::vec2{0.13, 0.19},
    /*31*/ glm::vec2{0.0975, 0.18},
    /*32*/ glm::vec2{0.0825, 0.165},
    /*33*/ glm::vec2{0.0225, 0.1725},
    /*34*/ glm::vec2{-0.2075, 0.06},
    /*35*/ glm::vec2{-0.2325, 0.0675},
    /*36*/ glm::vec2{-0.2525, 0.0475},
    /*37*/ glm::vec2{-0.2575, 0.0225},
    /*38*/ glm::vec2{-0.37, 0.3},
    /*39*/ glm::vec2{-0.4125, 0.34},
    /*40*/ glm::vec2{-0.4625, 0.2825},
    /*41*/ glm::vec2{-0.485, 0.24},
    /*42*/ glm::vec2{-0.4725, 0.205},
    /*43*/ glm::vec2{-0.4425, 0.17},
    /*44*/ glm::vec2{-0.3925, 0.1275},
    /*45*/ glm::vec2{-0.3525, 0.18},
    /*46*/ glm::vec2{-0.4425, 0.24},
    /*47*/ glm::vec2{-0.44, 0.27},
    /*48*/ glm::vec2{-0.4, 0.285},
    /*49*/ glm::vec2{-0.385, 0.2625},
    /*50*/ glm::vec2{-0.395, 0.215},
    /*51*/ glm::vec2{-0.415, 0.215},
    /*52*/ glm::vec2{-0.43, 0.255},
    /*53*/ glm::vec2{-0.415, 0.2675},
    /*54*/ glm::vec2{0.0175, 0.345},
    /*55*/ glm::vec2{-0.1875, 0.335},
    /*56*/ glm::vec2{0.1275, 0.0375},
    /*57*/ glm::vec2{0.165, 0.05},
    /*58*/ glm::vec2{0.165, 0.08},
    /*59*/ glm::vec2{0.135, 0.0825},
    /*60*/ glm::vec2{0.1175, 0.44},
    /*61*/ glm::vec2{0.135, 0.4675},
    /*62*/ glm::vec2{0.1225, 0.49},
    /*63*/ glm::vec2{0.0975, 0.5}};
/* Puntos de control:
 * 00-08: Contorno
 * 09-13: Barbilla
 * 14-19: Ojo izquierdo
 * 20-25: Ojo derecho
 * 26-33: Nariz
 * 34-37: Detalle-1-arco
 * 38-45: Oreja g3+g2+g2
 * 46-49: Detalle-1-arco-oreja
 * 50-53: Detalle-2-arco-oreja
 * 54   : Iris izquierdo
 * 55   : Iris derecho
 * 56-59: Ceja detalle
 * 60-63: Labio detalle
 */
glm::vec2 irisizq_original = p_control[54];
glm::vec2 irisder_original = p_control[55];

/* FUNCIONES */
void setupVertices(void);
void tess(vector<double> &contorno);
void printPuntosControl();
void buildAllCurves();
void display(GLFWwindow *window, double currentTime);
void init(GLFWwindow *window);
/*------------------------------ [CONTROLES] ---------------------------*/
int vertClickeado = -1;
bool edit_mode = false;
bool anim_ojos = false;
bool mode_mouse_coord = false;
bool opening_play = true;
bool clickPuntoControl(glm::vec2 center, float x, float y);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
/*----------------------------------------------------------------------*/

/*------------------------------ [IMGUI] -------------------------------*/
ImVec4 fondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImVec4 bart = ImVec4(217.0f / 255.0f, 199.0f / 255.0f, 10.0f / 255.0f, 1.00f); // original color bart
/*---------- ----------------------------------------------------------*/

int main()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Curvas-04-wcontrols: Mueva los puntos de control", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);
    cout << "GL_VERSION: " << glGetString(GL_VERSION) << endl;
    glfwSwapInterval(0);

    engine_audio = createIrrKlangDevice();
    if (!engine_audio){
        cout<<"Error engine audio"<<endl;
        return 0;
    }
    engine_audio->play2D("opening.mp3", true);

    init(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
    }
    engine_audio->drop(); // delete engine_audio

    glDeleteBuffers(numVBOs, vbo);
    glDeleteShader(renderingProgram);
    glDeleteVertexArrays(numVAOs, vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void tess(vector<double> &contorno)
{
    vector<double> aux(contorno.size());
    for (unsigned int i = 0; i <= contorno.size() - 4; i += 4)
    {
        aux[i] = contorno[i];
        aux[i + 1] = contorno[i + 1];
        aux[i + 2] = contorno[contorno.size() - 1 - i - 1];
        aux[i + 3] = contorno[contorno.size() - 1 - i];
    }
    if (contorno.size() % 2 != 0)
    {
        aux[contorno.size() - 1 - 1] = contorno[contorno.size() / 2 + 1];
        aux[contorno.size() - 1] = contorno[contorno.size() / 2 + 2];
    }
    contorno = aux;
}

void init(GLFWwindow *window)
{

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
    glUseProgram(renderingProgram);

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    setupVertices();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    glUniform2f(glGetUniformLocation(renderingProgram, "u_resolution"), (float)WIDTH, (float)HEIGHT);
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));
}

void setupVertices(void)
{

    // for(int i=0; i<p_control.size(); i++){
    //     p_control[i] = glm::scale(glm::mat4(1.0), glm::vec3(1, 1.6, 1)) * glm::vec4(p_control[i], 1, 1);
    // }

    buildAllCurves();
    glBindVertexArray(vao[0]);

    vector<double> p_control_points(p_control.size() * 2);
    for (int i = 0; i < p_control.size(); i++)
    {
        p_control_points[i * 2] = p_control[i].x;
        p_control_points[i * 2 + 1] = p_control[i].y;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, p_control_points.size() * sizeof(double), &p_control_points[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, contorno.size() * sizeof(double), &contorno[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, barbilla.size() * sizeof(double), &barbilla[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, ojo_izquierdo.size() * sizeof(double), &ojo_izquierdo[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, ojo_derecho.size() * sizeof(double), &ojo_derecho[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glBufferData(GL_ARRAY_BUFFER, nariz.size() * sizeof(double), &nariz[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glBufferData(GL_ARRAY_BUFFER, det_1_boca.size() * sizeof(double), &det_1_boca[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
    glBufferData(GL_ARRAY_BUFFER, oreja.size() * sizeof(double), &oreja[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
    glBufferData(GL_ARRAY_BUFFER, det_1_oreja.size() * sizeof(double), &det_1_oreja[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
    glBufferData(GL_ARRAY_BUFFER, det_2_oreja.size() * sizeof(double), &det_2_oreja[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
    glBufferData(GL_ARRAY_BUFFER, iris_izq.size() * sizeof(double), &iris_izq[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
    glBufferData(GL_ARRAY_BUFFER, iris_der.size() * sizeof(double), &iris_der[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
    glBufferData(GL_ARRAY_BUFFER, det_ceja.size() * sizeof(double), &det_ceja[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
    glBufferData(GL_ARRAY_BUFFER, det_labio.size() * sizeof(double), &det_labio[0], GL_STATIC_DRAW);
}

void buildAllCurves()
{
    contorno.clear();
    barbilla.clear();
    ojo_izquierdo.clear();
    ojo_derecho.clear();
    nariz.clear();
    det_1_boca.clear();
    oreja.clear();
    det_1_oreja.clear();
    det_2_oreja.clear();
    iris_der.clear();
    iris_izq.clear();
    det_ceja.clear();
    det_labio.clear();

    // Contorno
    contorno.push_back(p_control[8].x); // Añade el punto 8 para controlar el GL_TRIANGLE_FAN
    contorno.push_back(p_control[8].y); // Añade el punto 8 para controlar el GL_TRIANGLE_FAN
    addPointsCurveG2(contorno, p_control[0], p_control[1], p_control[2], 12);
    addPointsPelo(contorno, p_control[2], p_control[3], p_control[4], cantPelosBart * 2 + 1, alturaPelos); // 15/2 pelos aprox de largo 5
    addPointsCurveG2(contorno, p_control[4], p_control[5], p_control[6], 7);
    addPointsCurveG2(contorno, p_control[6], p_control[7], p_control[8], 7);

    // Barbilla
    barbilla.push_back(-0.3225); // añade para el triangle_fan
    barbilla.push_back(0.1475);  // añade para el triangle_fan
    addPointsCurveG2(barbilla, p_control[9], p_control[10], p_control[11], 8);
    addPointsCurveG2(barbilla, p_control[11], p_control[12], p_control[13], 8);
    barbilla.push_back(p_control[0].x); // añade para cerrar la figura al pintar con gltrianglefan
    barbilla.push_back(p_control[0].y); // añade para cerrar la figura al pintar con gltrianglefan

    // Ojo izquierdo
    ojo_izquierdo.push_back((p_control[14].x + p_control[17].x) / 2.0); // añade para el triangle_fan
    ojo_izquierdo.push_back((p_control[14].y + p_control[17].y) / 2.0); // añade para el triangle_fan
    addPointsCurveG3(ojo_izquierdo, p_control[14], p_control[15], p_control[16], p_control[17], 30);
    addPointsCurveG3(ojo_izquierdo, p_control[17], p_control[18], p_control[19], p_control[14], 30);
    ojo_izquierdo.push_back(p_control[14].x); // añade para cerrar la figura al pintar con gltrianglefan
    ojo_izquierdo.push_back(p_control[14].y); // añade para cerrar la figura al pintar con gltrianglefan

    // Ojo derecho
    ojo_derecho.push_back((p_control[20].x + p_control[23].x) / 2.0); // añade para el triangle_fan
    ojo_derecho.push_back((p_control[20].y + p_control[23].y) / 2.0); // añade para el triangle_fan
    addPointsCurveG3(ojo_derecho, p_control[20], p_control[21], p_control[22], p_control[23], 30);
    addPointsCurveG3(ojo_derecho, p_control[23], p_control[24], p_control[25], p_control[20], 30);
    ojo_derecho.push_back(p_control[20].x); // añade para cerrar la figura al pintar con gltrianglefan
    ojo_derecho.push_back(p_control[20].y); // añade para cerrar la figura al pintar con gltrianglefan

    // Nariz
    nariz.push_back(p_control[33].x); // añade para el triangle_fan
    nariz.push_back(p_control[33].y); // añade para el triangle_fan
    addPointsCurveG2(nariz, p_control[26], p_control[27], p_control[28], 10);
    addPointsCurveG3(nariz, p_control[28], p_control[29], p_control[30], p_control[31], 10);
    addPointsCurveG2(nariz, p_control[31], p_control[32], p_control[33], 10);

    // Detalle-1-curva-boca
    addPointsCurveG3(det_1_boca, p_control[34], p_control[35], p_control[36], p_control[37], 10);

    // Oreja
    addPointsCurveG2(oreja, p_control[38], p_control[39], p_control[40], 10);
    addPointsCurveG3(oreja, p_control[40], p_control[41], p_control[42], p_control[43], 10);
    addPointsCurveG2(oreja, p_control[43], p_control[44], p_control[45], 10);

    // Detalle-1-curva-boca
    addPointsCurveG3(det_1_oreja, p_control[46], p_control[47], p_control[48], p_control[49], 10);

    // Detalle-1-curva-boca
    addPointsCurveG3(det_2_oreja, p_control[50], p_control[51], p_control[52], p_control[53], 10);

    // Iris izquierdo
    addPointCircle(iris_izq, 0.015, p_control[54], 20);

    // Iris derecho
    addPointCircle(iris_der, 0.015, p_control[55], 20);

    // Detalle-Ceja
    addPointsCurveG3(det_ceja, p_control[56], p_control[57], p_control[58], p_control[59], 10);

    // Detalle-Labio
    det_labio.push_back(0.0325);
    det_labio.push_back(0.43);
    addPointsCurveG3(det_labio, p_control[60], p_control[61], p_control[62], p_control[63], 10);
}

void display(GLFWwindow *window, double currentTime)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2);
    // glDisable(GL_LINE_SMOOTH);

    ImGui::ColorEdit3("Color de fondo", (float *)&fondo);
    ImGui::ColorEdit3("Color de Bart Simpson", (float *)&bart);
    ImGui::Checkbox("Editar puntos de control (Presione 'E')", &edit_mode);
    if(ImGui::Checkbox("Animar ojos", &anim_ojos)){
        if (anim_ojos){
            p_control[54] = glm::vec2{0.0425,0.35};
            p_control[55] = glm::vec2{-0.17,0.3425};
        } else {
            p_control[54] = irisizq_original;
            p_control[55] = irisder_original;
        }
        setupVertices();
    }

    if(ImGui::Checkbox("Reproducir opening", &opening_play)){
        if(opening_play) engine_audio->play2D("opening.mp3", true);
        else engine_audio->stopAllSounds();
    }

    if (ImGui::SliderInt("Cantidad de pelos", &cantPelosBart, 3, 30))
    {
        setupVertices();
    }
    if (ImGui::SliderFloat("Altura de pelo", &alturaPelos, 0.0f, 10.0f))
    {
        setupVertices();
    }
    if (ImGui::Button("Imprimir puntos \nde control actuales \n(Presione 'P')"))
    {
        printPuntosControl();
    }

    ImGui::Separator();
    ImGui::Text("Promedio de aplicación %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();

    glClearColor(fondo.x, fondo.y, fondo.z, fondo.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(glGetUniformLocation(renderingProgram, "u_time"), (float)currentTime);
    glm::vec3 color;

    if (!edit_mode)
    {
        mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
        // mMat = glm::scale(mMat, glm::vec3(1.0, 1.0, 1.0));
        mMat *= transf;
        mvMat = vMat * mMat;

        float aspect = (float)WIDTH / (float)HEIGHT;
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    }
    else
    {
        mvMat = glm::mat4(1.0f);
        pMat = glm::mat4(1.0f);
    }
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(mvMat));


    /* ------------------ [PINTA BARBILLA] ------------------  */
    color = glm::vec3(bart.x, bart.y, bart.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, barbilla.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_STRIP, 1, barbilla.size() / 2 - 1); // omite el primer punto (q era solo para trianglefan)

    /* ------------------ [PINTA CONTORNO] ------------------ */
    color = glm::vec3(bart.x, bart.y, bart.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, contorno.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.1);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_STRIP, 1, contorno.size() / 2 - 1); // omite el primer punto (q era solo para trianglefan)

    /* ------------------ [DETALLE LABIO] ------------------ */
    color = glm::vec3(bart.x, bart.y, bart.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, det_labio.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_STRIP, 0, det_labio.size() / 2);


    /* ------------------ [PINTA OJO DERECHO] ------------------ */
    color = glm::vec3(1.0, 1.0, 1.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, ojo_derecho.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_LOOP, 1, ojo_derecho.size() / 2 - 1);

    /* ------------------ [PINTA OJO IZQUIERDO] ------------------ */
    color = glm::vec3(1.0, 1.0, 1.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, ojo_izquierdo.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_LOOP, 1, ojo_izquierdo.size() / 2 - 1);


    /* ------------------ [PINTA NARIZ] ------------------ */
    color = glm::vec3(bart.x, bart.y, bart.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nariz.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_STRIP, 1, nariz.size() / 2 - 1);

    /* ------------------ [DETALLE ARCO BOCA] ------------------ */
    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, det_1_boca.size() / 2);

    /* ------------------ [OREJA] ------------------ */
    color = glm::vec3(bart.x, bart.y, bart.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, oreja.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_STRIP, 0, oreja.size() / 2);

    /* ------------------ [DETALLE ARCO 1 OREJA] ------------------ */
    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, det_1_oreja.size() / 2);

    /* ------------------ [DETALLE ARCO 2 OREJA] ------------------ */
    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, det_2_oreja.size() / 2);

    // animacion de iris
    if (anim_ojos){

        glm::mat4 anim_ojo;
        double mousex, mousey;
        glfwGetCursorPos(window, &mousex, &mousey);
        mousex = (mousex) / (WIDTH / 2) - 1.0;
        mousey = ((mousey) / (HEIGHT / 2) - 1.0) * -1;
        mousex = mousex>+1?  1:mousex;
        mousex = mousex<-1? -1:mousex;
        mousey = mousey>+1?  1:mousey;
        mousey = mousey<-1? -1:mousey;
        // float angle = atan2(mousey,mousex);
        float move_x, move_y, rad;

        move_x = mousex * (fabs(p_control[14].x-p_control[23].x)/2.9) ;
        move_y = mousey * (fabs(p_control[14].x-p_control[23].x)/2.9) ;
        // if (mousex*mousex+mousey*mousey > rad*rad){
        //     move_x = cos(angle) * (rad/2.5) ;
        //     move_y = sin(angle) * (rad/2.5) ;
        // } else {

        // }
        if (!edit_mode)
        {
            anim_ojo = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
            anim_ojo *= transf;
            anim_ojo = glm::translate(anim_ojo, glm::vec3(move_x, move_y, 0));
            anim_ojo = vMat * anim_ojo;
        }
        else
        {
            anim_ojo = glm::translate(glm::mat4(1.0), glm::vec3(move_x, move_y, 0));
        }
        glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(anim_ojo));
    }
    /* ------------------ [IRIS IZQUIERDO] ------------------ */
    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, iris_izq.size() / 2);

    /* ------------------ [IRIS DERECHO] ------------------ */
    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, iris_der.size() / 2);

    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(mvMat));
    // termina animacion ojo

    /* ------------------ [DETALLE CEJA] ------------------ */
    color = glm::vec3(bart.x, bart.y, bart.z);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, det_ceja.size() / 2);

    color = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_STRIP, 0, det_ceja.size() / 2);

    if (edit_mode)
    {
        /*  [PINTA PUNTOS DE CONTROL]  */
        color = glm::vec3(0.0, 0.0, 0.1);
        glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color));
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glPointSize(5.0);
        glDrawArrays(GL_POINTS, 0, p_control.size());
    }


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void printPuntosControl()
{
    cout << "\tPuntos de control:" << endl;
    for (int i = 0; i < p_control.size(); i++)
    {
        cout << "/*" << i << "*/ ";
        cout << "glm::vec2";
        cout << "{" << p_control[i].x << "," << p_control[i].y << "}";
        if (i != p_control.size() - 1)
            cout << "," << endl;
    }
    cout << endl;
}

/*------------------------------ [CONTROLES] ---------------------------*/
bool clickPuntoControl(glm::vec2 center, float x, float y)
{
    float radio = 0.025; // radio del rango para clickear el punto de control
    x = x / (WIDTH / 2) - 1.0;
    y = (y / (HEIGHT / 2) - 1.0) * -1;
    float d = (center.x - x) * (center.x - x) + (center.y - y) * (center.y - y);
    d = sqrt(d);
    return d <= radio;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (mode_mouse_coord)
    {
        double posx, posy;
        glfwGetCursorPos(window, &posx, &posy);
        cout << "(" << (posx / (WIDTH / 2) - 1.0) << ", " << ((posy / (HEIGHT / 2) - 1.0) * -1) << ")" << endl;
    }
    if (!edit_mode)
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double posx, posy;
        glfwGetCursorPos(window, &posx, &posy);
        for (unsigned int i = 0; i < p_control.size(); i++)
        {
            if (clickPuntoControl(p_control[i], posx, posy))
                vertClickeado = i;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        vertClickeado = -1;
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (!edit_mode)
        return;
    if (vertClickeado != -1)
    {

        xpos = (xpos) / (WIDTH / 2) - 1.0;
        ypos = ((ypos) / (HEIGHT / 2) - 1.0) * -1;

        p_control[vertClickeado].x = xpos;
        p_control[vertClickeado].y = ypos;
        setupVertices();
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        // imprime los puntos de control actuales
        printPuntosControl();
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        // imprime los puntos de control actuales
        if (!edit_mode)
        {
            LOG("EDIT MODE ON");
            edit_mode = true;
        }
        else
        {
            LOG("EDIT MODE OFF");
            edit_mode = false;
        }
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        // imprime los puntos de control actuales
        if (!edit_mode)
        {
            LOG("CURSOR COORD MODE ON");
            mode_mouse_coord = true;
        }
        else
        {
            LOG("CURSOR COORD MODE OFF");
            mode_mouse_coord = false;
        }
    }

    double vel = 0.15 / zoom;
    if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(-vel, 0.0, 0.0));

    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(+vel, 0.0, 0.0));

    if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(0.0, vel, 0.0));

    if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(0.0, -vel, 0.0));
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    double sensibilidad = 1.05;
    if (yoffset != 0)
    {
        zoom = yoffset > 0 ? sensibilidad : 1 / sensibilidad;
        transf = glm::scale(transf, glm::vec3(zoom, zoom, zoom));
    }
}
