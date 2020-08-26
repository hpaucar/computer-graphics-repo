#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"
#include <string>

GLuint vbo_surface;
GLuint vbo_color;
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
            for (int k = 0; k < 4; k++) { // sumatoria en fórmula de Bezier
                B_i = bernstein(3, k, u);
                for (int l = 0; l < 4; l++) { // sumatoria en fórmula de Bezier
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
    for (int i = 0; i < ps.size(); i++) {
        points[i] = ps[i];
    }
    for (int i = 0; i < ps.size() / 3; i++) {
        colors[3 * i] = 0.0;
        colors[3 * i + 1] = 1.0;
        colors[3 * i + 2] = 0.0;
    }

    

    glGenBuffers(1, &vbo_surface);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_surface);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ps.size(), points, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ps.size(), colors, GL_STATIC_DRAW);

    

    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    if((vs = create_shader("basic3.v.glsl", GL_VERTEX_SHADER))==0) return false;
    if((fs = create_shader("basic3.f.glsl", GL_FRAGMENT_SHADER))==0) return false;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if(!link_ok){
        std::cout << "Problemas con el Shader" << std::endl;
        return false;
    }

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

    

    return true;
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'd':
            dx+= 0.05;
            break;
        case 'a':
            dx+= -0.05;
            break;
        case 'i':
            scale = scale * 1.02;
            break;
        case 'j':
            scale = scale * 0.98;
            break;
    }
    glutPostRedisplay(); 

}

void onDisplay(){
    //Creamos matrices de modelo, vista y proyeccion
    //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(dx, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(scale, scale, scale));




    glm::mat4 view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    glm::mat4 mvp = projection * view * model ;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(program);
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
    
    glDrawArrays(GL_POINTS, 0, numPoints);
    
    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_color);
    

    glutSwapBuffers();
    
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
    glutInit(&argc, argv);
    glutInitContextVersion(2,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("OpenGL");

    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK){
        std::cout << "Error inicializando GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!GLEW_VERSION_2_0){
        std::cout << "Tu tarjeta grafica no soporta OpenGL 2.0" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(init_resources()){
        glutDisplayFunc(onDisplay);
        glutReshapeFunc(onReshape);
        glutKeyboardFunc(keyboard); 
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutMainLoop();
    }

    free_resources();
    exit(EXIT_SUCCESS);
}
