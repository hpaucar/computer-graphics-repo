#version 330 core
out vec4 fColor;

void main()
{  
	if(gl_FragCoord.x > 400.0f && gl_FragCoord.y > 400.0f)
		fColor = vec4(0.0, 0.0, 1.0, 1.0); 
	else if(gl_FragCoord.x < 400.0f && gl_FragCoord.y < 400.0f)
		fColor = vec4(1.0, 0.0, 1.0, 1.0); 
	else
		fColor = vec4(1.0, 1.0, 1.0, 1.0); 	
}