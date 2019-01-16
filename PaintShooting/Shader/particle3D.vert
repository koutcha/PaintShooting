#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

layout(location = 4) in vec4 transform;

out vec3 P;
out vec3 N;
out vec2 UV;
out vec3 T;



uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec3 tempPos = position*transform.w;
	tempPos += transform.xyz;
	P = vec3(vec4(tempPos,1.0));
	N = normal;
	UV = uv;
	T = tangent;

	gl_Position = projection*view*vec4(P,1.0);
}
