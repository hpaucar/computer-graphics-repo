#version 430 core

layout(location=0) in vec4 position;

uniform mat4 transformation_matrix;

void main() {
    gl_Position = transformation_matrix * position;
}