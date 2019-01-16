#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

layout(location = 2) in vec4 transform;

layout(location = 3) in  int textureID;
layout(location = 4) in int teamIndex;

out vec3 P;
out vec4 PorjecitonPoint;
out vec2 UV;
out vec2 WUV;

flat out int TeamIndex; 

uniform float uvDivision;



uniform mat4 projection;
uniform mat4 view;

const float PI = 3.1415926535;

void main()
{
	vec2 tempPos = position.xz*transform.z;
	float angle = transform.w;
	mat2 rotation = mat2(cos(angle),sin(angle),-sin(angle),cos(angle));

	tempPos = rotation*tempPos;

	tempPos.x +=  transform.x;
	tempPos.y += transform.y;
	
	int divInt = int(uvDivision);
	float divWidth = 1.0/uvDivision;

	vec2 uvBias;

	uvBias.x = float((textureID%divInt))*divWidth;
	uvBias.y = float((textureID/divInt))*divWidth;

	
	P = vec3(tempPos.x,0,tempPos.y);
	UV = uv/uvDivision + uvBias;
	TeamIndex = teamIndex;
	vec4 proj = projection*view*vec4(P,1.0);

	WUV.x = proj.x /proj.w;
	WUV.y = proj.y /proj.w;

	gl_Position = proj;
}
