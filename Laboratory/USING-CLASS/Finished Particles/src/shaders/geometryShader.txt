#version 150

layout ( points ) in;
layout ( triangle_strip, max_vertices = 24 ) out;

in vec3 pass_colour[];

out vec3 finalColour;

uniform mat4 projectionViewMatrix;

const float size = 0.1;
const vec3 lightDirection = normalize(vec3(0.4, -1.0, 0.8));
const float ambientLighting = 0.3;

void createVertex(vec3 offset, vec3 colour){
	vec4 actualOffset = vec4(offset * size, 0.0);
	vec4 worldPosition = gl_in[0].gl_Position + actualOffset;
	gl_Position = projectionViewMatrix * worldPosition;
	finalColour = colour;
	EmitVertex();
}

vec3 calculateLighting(vec3 faceNormal){
	float brightness = max(dot(-lightDirection, faceNormal), ambientLighting);
	return pass_colour[0] * brightness;
}

void main(void){

	vec3 faceNormal = vec3(0.0, 0.0, 1.0);
	vec3 colour = calculateLighting(faceNormal);
	createVertex(vec3(-1.0, 1.0, 1.0), colour);
	createVertex(vec3(-1.0, -1.0, 1.0), colour);
	createVertex(vec3(1.0, 1.0, 1.0), colour);
	createVertex(vec3(1.0, -1.0, 1.0), colour);
	
	EndPrimitive();
	
	faceNormal = vec3(1.0, 0.0, 0.0);
	colour = calculateLighting(faceNormal);
	createVertex(vec3(1.0, 1.0, 1.0), colour);
	createVertex(vec3(1.0, -1.0, 1.0), colour);
	createVertex(vec3(1.0, 1.0, -1.0), colour);
	createVertex(vec3(1.0, -1.0, -1.0), colour);
	
	EndPrimitive();
	
	faceNormal = vec3(0.0, 0.0, -1.0);
	colour = calculateLighting(faceNormal);
	createVertex(vec3(1.0, 1.0, -1.0), colour);
	createVertex(vec3(1.0, -1.0, -1.0), colour);
	createVertex(vec3(-1.0, 1.0, -1.0), colour);
	createVertex(vec3(-1.0, -1.0, -1.0), colour);
	
	EndPrimitive();
	
	faceNormal = vec3(-1.0, 0.0, 0.0);
	colour = calculateLighting(faceNormal);
	createVertex(vec3(-1.0, 1.0, -1.0), colour);
	createVertex(vec3(-1.0, -1.0, -1.0), colour);
	createVertex(vec3(-1.0, 1.0, 1.0), colour);
	createVertex(vec3(-1.0, -1.0, 1.0), colour);
	
	EndPrimitive();
	
	faceNormal = vec3(0.0, 1.0, 0.0);
	colour = calculateLighting(faceNormal);
	createVertex(vec3(1.0, 1.0, 1.0), colour);
	createVertex(vec3(1.0, 1.0, -1.0), colour);
	createVertex(vec3(-1.0, 1.0, 1.0), colour);
	createVertex(vec3(-1.0, 1.0, -1.0), colour);
	
	EndPrimitive();
	
	faceNormal = vec3(0.0, -1.0, 0.0);
	colour = calculateLighting(faceNormal);
	createVertex(vec3(-1.0, -1.0, 1.0), colour);
	createVertex(vec3(-1.0, -1.0, -1.0), colour);
	createVertex(vec3(1.0, -1.0, 1.0), colour);
	createVertex(vec3(1.0, -1.0, -1.0), colour);
	
	EndPrimitive();

}