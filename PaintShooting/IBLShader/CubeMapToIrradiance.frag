#version 330
out vec4 fragment;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265369;

void main()
{
	//�����̒��S���璸�_�ɐL�т�x�N�g��
	vec3 normal = normalize(localPos);
	//���ˏƓx
	vec3 irradiance = vec3(0.0);
	//�t���O�����g�̐ڋ�Ԃ̐��K�������
	vec3 up = vec3(0.0,1.0,0.0);
	vec3 right = cross(up,normal);
	up = cross(normal,right);
	
	//sampleDelta���Ŕ����ɂ��Ă̐ϕ��𗣎U�I�ɋߎ�����
	float sampleDelta = 0.025;
	//�T���v�����O��
	float nrSamples = 0.0;
	//�P�ʋ����W�Řa�����߂� 
	for(float phi = 0.0; phi<2.0*PI; phi+= sampleDelta)
	{
		for(float theta = 0.0; theta<0.5*PI; theta += sampleDelta)
		{
			//�ڋ�Ԃł̃T���v�����O�x�N�g��
			vec3 tangentSample = vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			//�ڋ�Ԃ��烏�[���h��Ԃ֕ϊ�
			vec3 sampleVec = tangentSample.x*right+tangentSample.y*up+tangentSample.z*normal;

			
			//��ɍs���قǖʐς��������Ȃ邱�ƁA�@���Ƃ̊p�x��������ƌ������邱�Ƃ��l������
			irradiance += texture(environmentMap,sampleVec).rgb*cos(theta)*sin(theta);
			nrSamples++;
		}
	
	}
	//���ς��ĒP�ʖʐϓ������irradiance�����߂āA�ʐς�������Ƃ��̎��ɂȂ�
	irradiance = PI*irradiance*(1.0/nrSamples);

	//�@��n�����̃t���O�����g�ɑ΂�����ˏƓx���e�N�X�`���Ƀ����_�����O����
	fragment = vec4(irradiance,1.0);
}
