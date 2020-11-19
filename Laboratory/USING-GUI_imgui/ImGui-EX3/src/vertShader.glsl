#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec2 textCoord;

out vec2 tc;

uniform mat4 mv_matrix; // variable global cuyo valor podemos pasar desde C++
uniform mat4 proj_matrix; // variable global cuyo valor podemos pasar desde C++

layout (binding=0) uniform sampler2D samp;

void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0); // determina la posicion de cada fragmento o pixel
    tc = textCoord;
}