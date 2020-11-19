#version 430

layout (location=0) in vec3 position; // recive del buffer 0 los datos de position

uniform mat4 mv_matrix; // variable global cuyo valor podemos pasar desde C++
uniform mat4 proj_matrix; // variable global cuyo valor podemos pasar desde C++

out vec4 varyingColor; // variable out, dicha variable se puede atrapar desde otro codigo glsl como en fragShader.glsl

void main() {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0); // determina la posicion de cada fragmento o pixel
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5); // establece el valor de la variable out 
}