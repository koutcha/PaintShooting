#version 330
layout (location= 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 localPos;

void main()
{
	localPos = position;
	//�ړ����������O
	mat4 rotationView = mat4(mat3(view));

	//mat4 rotationView = view;
	vec4 clipPos = projection*rotationView*vec4(localPos,1.0);

	//xyww�Ƃ����depth�l�����1.0
	gl_Position = clipPos.xyww;
}