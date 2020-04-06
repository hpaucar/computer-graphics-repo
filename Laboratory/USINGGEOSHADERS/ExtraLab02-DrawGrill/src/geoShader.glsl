#version 330
layout (points) in;
layout (line_strip, max_vertices = 3) out;
//layout(points, max_vertices = 3) out;
//layout(triangle_strip, max_vertices = 3) out;

in vec3 vColor[];

out vec3 fColor;

const float r = 10.0;
const float r1 = 0.0; //Revisar solo funciona con 0 o 1
const float len = 3.0;

void build_house(vec4 position){  
  	fColor = vColor[0]; 	
    
    //gl_Position = position + vec4(-5.0, 0.0, 5.0, 0.0);    	// 1:bottom-left
    //fColor = vec3(1.0, 0.0, 0.0);
    //EmitVertex();      
    //gl_Position = position + vec4( 5.0, 0.0, -5.0, 0.0);    // 4:top-right
    //fColor = vec3(1.0, 1.0, 1.0);
    //EmitVertex();
    //gl_Position = position + vec4(-5.0, 0.0, -5.0, 0.0);    // 3:top-left
    //fColor = vec3(0.0, 0.0, 1.0);
    //EmitVertex(); 
    //gl_Position = position + vec4( 5.0, 0.0, 5.0, 0.0);  	// 2:bottom-right
    ////fColor = vec3(0.0, 1.0, 0.0);
    //EmitVertex(); 

    gl_Position = position + vec4(-len, 0, r1, 0.0);    // 1:bottom-left
    fColor = vec3(1.0, 0.0, 0.0);
    EmitVertex();   
    gl_Position = position + vec4( len, 0, r1, 0.0);    // 2:bottom-right
    fColor = vec3(0.0, 1.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(-len/2, 0, 10, 0.0);    // 3:top-left
    fColor = vec3(0.0, 0.0, 1.0);
    EmitVertex();
    
    
    //gl_Position = position + vec4( len,  len, r1, 0.0);    // 4:top-right
    //fColor = vec3(1.0, 1.0, 1.0);
    //EmitVertex();

            
    EndPrimitive();
}

void main() {   
	
    build_house(gl_in[0].gl_Position);
} 