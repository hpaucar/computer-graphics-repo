#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;	
layout (location = 1) in vec3 vColor;							  
uniform mat4 model;                                                           
out vec4 vertexColor;                                                                              
                                                                              
void main()                                                                   
{                                                                             
    gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);
    vertexColor = vec4(vColor,1.0);		  
}