#version 330

//���_���
in vec3 P;
in vec3 N;
in vec2 UV;
in vec3 T;

out vec4 fragment;

//���_�ʒu
uniform vec3 viewPos;

//�ގ��f�[�^
uniform vec3 albedo;
uniform float roughness;
uniform float metallic;
uniform float ao;

//���C�g�̐�
const int MAX_LIGHT = 4;

//���C�g�̈ʒu�A�F�A����
uniform vec3 PLpos[MAX_LIGHT];
uniform vec3 PLcolor[MAX_LIGHT];
uniform float PLintensity[MAX_LIGHT];


const float PI = 3.1415926;




//�@�����z�֐�D
//�n�[�t�x�N�g���ɉe������悤�ɐ��񂳂�Ă���}�C�N���t�@�Z�b�g�̗ʂ��ߎ�����
//�}�C�N���t�@�Z�b�g���ʂ̖@���ɂǂ̂悤�ɂ���ĕ��z���Ă��邩�𓝌v�I�ɋ��߂�
//�}�C�N���t�@�Z�b�g�ߎ��ōł��d�v�ȕ���
//N:�@���@H:�n�[�t�x�N�g���@a �e���p�����[�^�[
float DistributionGGX(vec3 Normal,vec3 H,float a)
{
	float a2 = a*a;
	float NdotH =  max(dot(Normal,H),0.0f);

	float NdotH2 = NdotH*NdotH;

	//Trowbridge-Reitz model

	float nom = a2;
	float denom = PI*(1-(1-a2)*NdotH2);
	denom = denom*denom;

	return nom/denom;
}

//�􉽊֐�G
//�}�C�N���t�@�Z�b�g�ɂ���Ď�������B��镔��������͂����A�ɂȂ镔�����L�q����
//�\�ʂ��e���Ƃ��Ƀ}�C�N���t�@�Z�b�g�����̃}�C�N���t�@�Z�b�g�ɉe�𗎂Ƃ��A���ʂƂ��ĕ\�ʂ̔��ˌ�����������
//�܂����˂����������̃}�C�N���t�@�Z�b�g���Ղ��Ď����ɓ͂��Ȃ��ꍇ������

//NdotV(NdotL)�@���ρ@k:roughness���狁�߂�萔��
float GeometrySchlickGGX(float NdotV,float k)
{
	float nom = NdotV;
	float denom = NdotV*(1.0-k) +k;

	return nom / denom;

}
float GeometrySmith(vec3 N,vec3 V,vec3 L,float k)
{
	float NdotV = max(dot(N,V),0.0);
	float NdotL = max(dot(N,L),0.0);

	//��������̎Օ�
	float ggx1 = GeometrySchlickGGX(NdotV,k);
	//��������̎Օ�
	float ggx2 = GeometrySchlickGGX(NdotL,k);

	return ggx1*ggx2;

}
//�t���l��������
//���ˊp�Ƌ��ܗ�����ĕς�锽�˗����L�q����
//F0:�ގ��ɂ���Č��܂�ϐ�
//F0�����O�v�Z�ŋ��߂邱�Ƃɂ���ċ����Ɣ�����œ��������������p�ł���
vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
	//schilick�ߎ�
	return F0 + (1.0 - F0)*pow(1.0 - cosTheta,5.0);
}

void main()
{	
	
	vec3 Normal = normalize(N);
	vec3 V = normalize(viewPos - P);

	vec3 Lo = vec3(0.0);

	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	for(int i=0; i<MAX_LIGHT; ++i)
	{
		//���ˋP�x�̌v�Z(input)
		vec3 L = normalize(PLpos[i]  - P);
		vec3 H = normalize(V+L);

		float d = length(PLpos[i] - P);
		float attenuation = 1.0/ (d*d);

		vec3 radiance = PLcolor[i]*PLintensity[i]*attenuation;

		//�N�b�N�g�����Xbrdf
		float D = DistributionGGX(Normal,H,roughness);
		float G = GeometrySmith(Normal,V,L,roughness);
		vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
		//�g�U���Ƌ��ʔ��ˌ��ŃG�l���M�[�ۑ�
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		//�����ɋz��������
		kD *= 1.0 - metallic;

		vec3 nom = D*F*G;

		float denom = 4.0*max(dot(Normal,V),0.0)*max(dot(Normal,L),0.0);

		//���ʔ��ˌ�(�O����h�~��0.0001)
		vec3 specular = nom/max(denom,0.0001);

		//���ˋP�x(output)
		float NdotL = max(dot(Normal,L),0.0);
		Lo += (kD*albedo/PI + specular)*radiance*NdotL;

	}

	vec3 ambient = vec3(0.03)*albedo*ao;
	vec3 color = ambient+ Lo;

	//HDR�}�b�s���O
	color = color/(color + vec3(1.0));
	//�K���}�␳
	color = pow(color,vec3(1.0/2.2));

	fragment = vec4(color,1.0f);
}