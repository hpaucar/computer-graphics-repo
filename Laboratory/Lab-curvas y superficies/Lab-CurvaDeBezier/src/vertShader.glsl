#version 330 core
layout (location = 0) in vec2 iPosition;

float s = 3.0;

void main(){	
   gl_Position = vec4(s*iPosition.x, s*iPosition.y, 0.0, 1.0);
}