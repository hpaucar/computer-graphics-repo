#version 330 core

// Input vertex data and color data 
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;

// Output fragment data 
out vec3 fragmentColor;

void main()
{
	// position of each vertex in homogeneous coordinates
	//gl_PointSize = 10.0;
    //gl_Position.xyz = vertexPosition;
    //gl_Position.w = 1.0;
	gl_Position = mvp * vec4(vertexPosition, 1.0);
	
	// the vertex shader just passes the color to fragment saheder
	fragmentColor = vertexColor;

}

