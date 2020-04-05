#version 330 core
layout (location = 1) in vec3 vColor;
layout (location = 0) in vec4 vPosition;

out vec4 vertexColor;
void main()
{	gl_Position = vPosition;
	vertexColor = vec4(vColor,1.0); }