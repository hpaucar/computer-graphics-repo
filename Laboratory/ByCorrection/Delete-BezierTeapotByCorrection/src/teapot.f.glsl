#version 410 core
in vec3 f_color;
out vec4 color;
void main(void) {
  color = vec4(f_color.xyz, 1.0);
}

