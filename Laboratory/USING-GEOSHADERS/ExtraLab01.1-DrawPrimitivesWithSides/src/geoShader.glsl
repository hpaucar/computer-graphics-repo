#version 150

layout(points) in;
layout(line_strip, max_vertices = 64) out;

in vec3 vColor[]; //Posee un unico indice = [0]
in float vSides[]; //Posee un unico indice = [0]
out vec3 fColor;

const float PI = 3.1415926;

void main(){
	fColor = vColor[0];
	
	// Safe, GLfloats can represent small integers exactly
	for (int i = 0; i <= vSides[0]; i++) {
		// Angle between each side in radians
		float ang = PI * 2.0 / vSides[0] * i;
		
		// Offset from center of point (0.3 to accomodate for aspect ratio)
		vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
		gl_Position = gl_in[0].gl_Position + offset;
		
		EmitVertex();
	}
	EndPrimitive();
}