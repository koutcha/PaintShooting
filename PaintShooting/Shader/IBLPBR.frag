#version 330

//���_���
in vec3 P;
in vec3 N;
in vec2 UV;
in vec3 T;

out vec4 fragment;

//���_�ʒu
uniform vec3 viewPos;

//�e�N�X�`��on off
uniform bool useMaterialTexture;

//���ڌ�(�_�C���N�g���C�e�B���O)
//���C�g�̐�
const int MAX_LIGHT = 4;

//���C�g�̈ʒu�A�F�A����
uniform vec3 PLpos[MAX_LIGHT];
uniform vec3 PLcolor[MAX_LIGHT];
uniform float PLintensity[MAX_LIGHT];

//hdr�␳�l
uniform float exposure;

//�Ԑڌ�(�摜�x�[�X���C�e�B���O)

uniform sampler2D brdfLUT;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;

const float PI = 3.1415926;

//�ގ��f�[�^

//���l�^
uniform vec3 albedoMaterial;
uniform float roughnessMaterial;
uniform float metallicMaterial;
uniform float aoMaterial;

//�e�N�X�`���^
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;




vec3 getNormalFromMap()
{
	//�e�N�X�`�����W�̖@��
	vec3 tangentNormal = texture(normalMap,UV).rgb*2.0 - 1.0;

	//�ڋ�Ԃ����߂�
	vec3 Normal = normalize(N);
	vec3 Tangent = normalize(T);
	vec3 Bitangent = cross(Normal,Tangent);
	//���[���h��Ԃ֕ϊ�
	mat3 TBN = mat3(Tangent,Bitangent,Normal);

	return normalize(TBN*tangentNormal);
}


//�@�����z�֐�D
//�n�[�t�x�N�g���ɉe������悤�ɐ��񂳂�Ă���}�C�N���t�@�Z�b�g�̗ʂ��ߎ�����
//�}�C�N���t�@�Z�b�g���ʂ̖@���ɂǂ̂悤�ɂ���ĕ��z���Ă��邩�𓝌v�I�ɋ��߂�
//�}�C�N���t�@�Z�b�g�ߎ��ōł��d�v�ȕ���
//N:�@���@H:�n�[�t�x�N�g���@a �e��
float DistributionGGX(vec3 Normal,vec3 H,float roughness)
{
	float a = roughness*roughness;
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
float GeometrySchlickGGX(float NdotV,float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r)/8.0;
	float nom = NdotV;
	float denom = NdotV*(1.0-k) +k;

	return nom / denom;

}
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness)
{
	float NdotV = max(dot(N,V),0.0);
	float NdotL = max(dot(N,L),0.0);

	//��������̎Օ�
	float ggx1 = GeometrySchlickGGX(NdotV,roughness);
	//��������̎Օ�
	float ggx2 = GeometrySchlickGGX(NdotL,roughness);

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
vec3 fresnelSchlickRoughness(float cosTheta,vec3 F0,float roughness)
{
	 return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}



void main()
{	
	vec3 Normal = useMaterialTexture ? getNormalFromMap() :normalize(N);
	vec3 albedo = useMaterialTexture ? texture(albedoMap,UV).rgb : albedoMaterial;
	float metallic = useMaterialTexture ? texture(metallicMap,UV).r : metallicMaterial;
	float roughness = useMaterialTexture ? texture(roughnessMap,UV).r : roughnessMaterial;
	float ao = useMaterialTexture ? texture(aoMap,UV).r : aoMaterial;

	vec3 V = normalize(viewPos - P);
	vec3 R = reflect(-V, Normal);


	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	
	vec3 Lo = vec3(0.0);

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
		//max(dot(H,V),0.0)�ł͂Ȃ�clamp(dot(H,V),0.0,1.0);
		vec3 F = fresnelSchlick(clamp(dot(H,V),0.0,1.0),F0);
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

		


	//���}�b�v�̊Ԑڌ��ɂ��Ă̌v�Z��ǉ��A�����̌v�Z��ύX
	vec3 F = fresnelSchlickRoughness(max(dot(Normal, V), 0.0), F0, roughness); 
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= (1.0 - metallic);	


	vec3 irradiance = texture(irradianceMap, Normal).rgb;
	vec3 diffuse  = irradiance *albedo;

	//���}�b�v����̋��ʔ��˂̌v�Z
	const float MAX_REFLECTION_LOD = 4.0;
	//Lod�Y��Ȃ���
	vec3 prefilteredColor = textureLod(prefilterMap,R,roughness*MAX_REFLECTION_LOD).rgb;
	vec2 envBRDF = texture(brdfLUT,vec2(max(dot(Normal,V),0.0),roughness)).rg;
	//�X�y�L�����̐ϕ��v�Z��
	vec3 specular =  prefilteredColor*(F*envBRDF.x+envBRDF.y);

	vec3 ambient    = (kD * diffuse+specular) * ao;
	//Lo�������ō��_�m�C�Y������
	vec3 color =ambient + Lo;


	//HDR�}�b�s���O
	color = vec3(1.0) - exp(-color*exposure);
	//�K���}�␳
	color = pow(color,vec3(1.0/2.2));

	fragment = vec4(color,1.0f);
}