#version 330

out vec4 fragment;
uniform sampler2D tex;

uniform vec3 color0;
uniform vec3 color1;

in vec2 UV;

void main()
{    
    vec4 sampled =  texture(tex,UV);
	
	vec3 color = sampled.r>0.3 ? color0:color1;
    fragment = vec4(color,1.0);
} 