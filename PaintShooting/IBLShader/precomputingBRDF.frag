#version 330
out vec2 fragment;
in vec2 UV;
//BRDF�̐ϕ����ʂ����O�v�Z����

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
// ----------------------------------------------------------------------------
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
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0)*Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	//�����W����f�J���g���W�n�ɕϊ�
	vec3 H;
	H.x = cos(phi)*sinTheta;
	H.y = cos(phi)*sinTheta;
	H.z = cosTheta;

	//�ڋ�Ԃ��烏�[���h���W�n�ɕϊ�
	//���s��̃����N������Ȃ��悤��up�x�N�g�������߂�
	vec3 up =abs(N.z) < 0.999?vec3(0.0,0.0,1.0):vec3(1.0,0.0,0.0);
	vec3 tangent = normalize(cross(up,N));
	vec3 bitangent = cross(N,tangent);
	vec3 sampleVec = tangent*H.x + bitangent*H.y + N*H.z;

	return normalize(sampleVec);
}

//�􉽊֐�G�@�}�C�N���t�@�Z�b�g�ŎՒf���������̋ߎ�
float GeometrySchlickGGX(float NdotV,float roughness)
{
	float a = roughness;

	//IBL�̏ꍇ��k = (roughnes+1)^2/8�ł͂Ȃ�k= roughness^2/2
	float k = (a*a)/2.0;
	float nom = NdotV;
	float denom = NdotV*(1.0 - k)+k;
	return nom/denom;
}
//���˂Ɣ��˗����ŋN����Ւf���l������
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness)
{
	//����
	float NdotV = max(dot(N,V),0.0);
	float ggx2 = GeometrySchlickGGX(NdotV,roughness);
	//����
	float NdotL = max(dot(N,L),0.0);
	float ggx1 = GeometrySchlickGGX(NdotL,roughness);

	return ggx1*ggx2;
}


//NdotV�������Aroughness���c���Ɏ�莖�O�v�Z���ʂ��e�N�X�`���ɏĂ��t����
vec2 IntegrateBRDF(float NdotV,float roughness)
{
	//���炭N��V�̂Ȃ��p(NdotV)�������ɂ��Ȃ��̂�y�����̐����ɂ��čl����K�v���Ȃ�
	vec3 V;
	V.x = sqrt(1.0 - NdotV*NdotV);
	V.y = 0.0;
	V.z = NdotV;

	float A = 0.0;
	float B = 0.0;
	//N�͌Œ肷��
	vec3 N = vec3(0.0,0.0,1.0);
	
	const uint SAMPLE_COUNT = 1024u;
	for(uint i=0u; i<SAMPLE_COUNT; ++i)
	{
		vec2 Xi = Hammersley(i,SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi,N,roughness);
		vec3 L = normalize(2.0*dot(V,H)*H-V);
		//N=(0.0,0.0,1.0)�ł��邱�Ƃɂ����ς�Z�����̒l�ƈ�v����
		float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

		if(NdotL>0.0)
		{
			//BRDF�̌v�Z
			float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);
			//�ϕ��̎��ό`�Ŏ���2���ɕ�����A��ꍀ��A�A��񍀂�B�ɂ��ꂼ��i�[����
            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
		}	
	}
	A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);

	return vec2(A,B);
}

void main() 
{
    vec2 integratedBRDF = IntegrateBRDF(UV.x,UV.y);
    fragment = integratedBRDF;
}