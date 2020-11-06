#version 430

out vec4 color;

uniform vec3 u_color;

in vec4 fcolor;

void main(void) {
    color = fcolor;
    // color = vec4(0.75, 0.7, 0.9, 1);
}