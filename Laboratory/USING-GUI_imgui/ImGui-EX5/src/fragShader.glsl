#version 330 core
out vec4 color_out;
in vec3 the_color;

uniform float u_time;
uniform vec2 u_resolution;

void main()
{
    /*
     *----- GRADIENT ------
     * from: https://stackoverflow.com/questions/47285778/how-to-create-a-mutli-step-radial-gradient-fragment-shader
     */   
    vec2 pos_ndc = 2.0 * gl_FragCoord.xy / u_resolution.xy - 1.0;
    float dist = length(pos_ndc);

    float step1 = 0.0;
    float step2 = 0.33;
    float step3 = 0.66;
    float step4 = 1.0;
    dist *= 0.55;
    vec4 color = mix(vec4(1), vec4(1), smoothstep(step1, step2, dist));
    color = mix(color, vec4(0), smoothstep(step2, step3, dist));
    color = mix(color, vec4(1), smoothstep(step3, step4, dist));
    /* ----- END CODE GRADIENT*/

    // BALLS
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    float time = u_time;
    st = st * (1-st); // DOBLE LINEA

    float grosor = 0.6 * sin(time);

    float s_y = smoothstep(abs(sin(time)) - grosor,abs(sin(time)),st.x) - 
              smoothstep(abs(sin(time)), abs(sin(time)) + grosor,st.x);

    color +=  s_y;
    color_out = vec4 (the_color.xyz * color.xyz, 1);

    // color_out = vec4(the_color,1);
}