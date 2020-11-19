#version 330 core

layout (location = 0) in vec3 position;

out vec3 the_color;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec3 u_color;
uniform mat4 mv_matrix; // variable global cuyo valor podemos pasar desde C++
uniform mat4 proj_matrix;

void main(void) {
    // gl_Position = vec4(position, 1.0);
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    // gl_Position = mv_matrix * vec4(position, 1.0);
    the_color = u_color;
}
