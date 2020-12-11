#version 330 core
layout (location = 0) in vec4 vPosition;
void main()
{	gl_Position =vPosition; 
	//gl_Position = vec4(0.5*vPosition.x, 0.5*vPosition.y, vPosition.z, 1);
	
}