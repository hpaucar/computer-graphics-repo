#version 150
in vec2 iPosition;
in vec3 iColor;
out vec3 oColor;

void main(){
   oColor = iColor;
   gl_Position = vec4(iPosition, 0.0, 1.0);
}