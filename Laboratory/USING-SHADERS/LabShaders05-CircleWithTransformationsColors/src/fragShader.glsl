#version 410

in vec3 oColor;
out vec4 color;

void main(void){
	color = vec4(oColor, 1.0);
	//if(gl_FragCoord.x > 400)
	//	color = vec4(0.0, 0.0, 1.0, 1.0);
	//else
	//	color = vec4(1.0, 0.0, 0.0, 1.0);	
}
