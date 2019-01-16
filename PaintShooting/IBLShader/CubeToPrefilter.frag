#version 330

out vec4 fragment;

in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

const float PI = 3.14159265359;


//�������e�J�����@�̂��߂̋K���I�ł��邪��l�Ȓ�H���Ⴂ�ʗ�𐶐�����
//���������������͉����邪���ւ̎���������
float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}  
//�\�ʂ̑e���ɑ΂��ăT���v�����O����}�C�N���T�[�t�F�X�܂��̃n�[�t�x�N�g���̔z�������߂�(�T���v�����O�x�N�g����Ԃ�)
//�e����΋��ʃ��[�u�i�n�[�t�x�N�g���̍L������j���L���Ȃ�
vec3 ImportanceSampleGGX(vec2 Xi,vec3 N,float roughness)
{
	float a = roughness*roughness;

	float phi = 2.0*PI*Xi.x;
	float cosTheta = sqrt( (1.0 - Xi.y)  /  (1.0 + (a*a - 1.0) * Xi.y  )  );
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	//�����W����f�J���g���W�n�ɕϊ�
	vec3 H;
	H.x = cos(phi)*sinTheta;
	H.y = sin(phi)*sinTheta;
	H.z = cosTheta;

	//�ڋ�Ԃ��烏�[���h���W�n�ɕϊ�
	//���s��̃����N������Ȃ��悤��up�x�N�g�������߂�
	vec3 up =abs(N.z) < 0.999?vec3(0.0,0.0,1.0):vec3(1.0,0.0,0.0);
	vec3 tangent = normalize(cross(up,N));
	vec3 bitangent = cross(N,tangent);
	vec3 sampleVec = tangent*H.x + bitangent*H.y + N*H.z;

	return normalize(sampleVec);
}

void main()
{
	//N=V�Ɖ��肷��(���O�v�Z�̂��ߎ����x�N�g���Ɉˑ����Ȃ��悤�ɂ���j
	vec3 N = normalize(localPos);
	vec3 R = N;
	vec3 V = R;
	//�T���v�����O����x�N�g����
	const uint SAMPLE_COUNT = 1024u;
	float totalWeight = 0.0;
	vec3 prefilteredColor = vec3(0.0);

	//�������e�J�����ϕ�
	//sum�����߂�
	for(uint i=0u; i<SAMPLE_COUNT; ++i)
	{
		vec2 Xi = Hammersley(i,SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi,N,roughness);
		vec3 L = normalize(2.0*dot(V,H)*H-V);
		float NdotL = max(dot(N, L), 0.0);
		if(NdotL>0.0)
		{
			//�������N�G���[�̌���(texture�֐�) .rgb���Ȃ��ƃ_���炵��
			prefilteredColor += texture(environmentMap,L).rgb*NdotL;
			totalWeight += NdotL;
		}	
	}
	//�e���̏��Ȃ��T���v��(NdotL�̏������j������^���Ă��Ȃ��A�ŏI�d�ʁi�d�݂̍��v�j��prefiteredColor������
	//cosTheta���Ȃ킿NdotL�ŏd�ݕt�����邱�Ƃɂ����ǂ����ʂ�����ꂽ�炵��
	prefilteredColor = prefilteredColor/totalWeight;

	fragment = vec4(prefilteredColor,1.0);
}