#ifndef UTILS_H
#define UTILS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

#include <string>
#include <fstream>
#include <iostream>

namespace Utils{
    std::string readShader(const char *filePath) {
        std::string content;
        std::ifstream fileStream(filePath, std::ios::in);
        std::string line = "";

        while(getline(fileStream, line)) {
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }
    unsigned int createShaderProgram(const char *file_vs, const char *file_fs ) {
        std::string str_src_vs = readShader(file_vs);
        std::string str_src_fs = readShader(file_fs);

        const char *src_vs = str_src_vs.c_str();
        const char *src_fs = str_src_fs.c_str();

        unsigned int vs, fs;
        vs = glCreateShader(GL_VERTEX_SHADER);
        fs = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vs, 1, &(src_vs), NULL);
        glShaderSource(fs, 1, &(src_fs), NULL);

        glCompileShader(vs);
        glCompileShader(fs);

        unsigned int program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        return program;
    }

    GLuint loadTexture(const char *textImagePath) {
    GLuint textureID;
    textureID = SOIL_load_OGL_texture(textImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == 0) std::cout<<"could not find texture file "<<textImagePath<<std::endl;
    return textureID;
    }

}
#endif
