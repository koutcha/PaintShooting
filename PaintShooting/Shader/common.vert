#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

out vec3 P;
out vec3 N;
out vec2 UV;
out vec3 T;



uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



void main()
{
	P = vec3(model*vec4(position,1.0));
	N = mat3(model)*normal;
	UV = uv;
	T = mat3(model)*tangent;

	gl_Position = projection*view*vec4(P,1.0);
}
