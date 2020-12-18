#version 330
//INPUT layout qualifier
------------------------- 
//points: when drawing GL_POINTS primitives (1).
//lines: when drawing GL_LINES or GL_LINE_STRIP (2).
//lines_adjacency: GL_LINES_ADJACENCY or GL_LINE_STRIP_ADJACENCY (4).
//triangles: GL_TRIANGLES, GL_TRIANGLE_STRIP or GL_TRIANGLE_FAN (3).
//triangles_adjacency : GL_TRIANGLES_ADJACENCY or GL_TRIANGLE_STRIP_ADJACENCY (6).
layout (points) in; //indica el tipo de primitiva GL_POINTS de entrada

//OUTPUT layout qualifier
-------------------------
//points
//line_strip
//triangle_strip
layout (triangle_strip, max_vertices = 5) out; //indica el tipo de primitiva GL_TRIANGLE_STRIP de salida 
//layout(line_strip, max_vertices = 5) out;

in vec3 vColor[];

out vec3 fColor;

void build_house(vec4 position){  
  	fColor = vColor[0]; 	
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();    
    
    EndPrimitive();
}

void main() {   
	
    build_house(gl_in[0].gl_Position);
} 