#version 330

out vec4 fragment;
uniform sampler2D tex;

in vec2 UV;

void main()
{    
    vec4 sampled =  texture(tex,UV);
    fragment = sampled;
} 