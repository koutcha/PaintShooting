#version 330

//頂点情報
in vec3 P;
in vec3 N;
in vec2 UV;
in vec3 T;

out vec4 fragment;

//視点位置
uniform vec3 viewPos;

//材質データ
uniform vec3 albedo;
uniform float roughness;
uniform float metallic;
uniform float ao;

//ライトの数
const int MAX_LIGHT = 4;

//ライトの位置、色、強さ
uniform vec3 PLpos[MAX_LIGHT];
uniform vec3 PLcolor[MAX_LIGHT];
uniform float PLintensity[MAX_LIGHT];


const float PI = 3.1415926;




//法線分布関数D
//ハーフベクトルに影響するように整列されているマイクロファセットの量を近似する
//マイクロファセットが面の法線にどのようにずれて分布しているかを統計的に求める
//マイクロファセット近似で最も重要な部分
//N:法線　H:ハーフベクトル　a 粗さパラメーター
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

//幾何関数G
//マイクロファセットによって視線から隠れる部分や光が届かず陰になる部分を記述する
//表面が粗いときにマイクロファセットが他のマイクロファセットに影を落とし、結果として表面の反射光が減少する
//また反射した光が他のマイクロファセットい遮られて視線に届かない場合もある

//NdotV(NdotL)　内積　k:roughnessから求める定数項
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

	//視線からの遮蔽
	float ggx1 = GeometrySchlickGGX(NdotV,k);
	//光源からの遮蔽
	float ggx2 = GeometrySchlickGGX(NdotL,k);

	return ggx1*ggx2;

}
//フレネル方程式
//入射角と屈折率よって変わる反射率を記述する
//F0:材質によって決まる変数
//F0を事前計算で求めることによって金属と非金属で同じ方程式が利用できる
vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
	//schilick近似
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
		//放射輝度の計算(input)
		vec3 L = normalize(PLpos[i]  - P);
		vec3 H = normalize(V+L);

		float d = length(PLpos[i] - P);
		float attenuation = 1.0/ (d*d);

		vec3 radiance = PLcolor[i]*PLintensity[i]*attenuation;

		//クックトランスbrdf
		float D = DistributionGGX(Normal,H,roughness);
		float G = GeometrySmith(Normal,V,L,roughness);
		vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
		//拡散光と鏡面反射光でエネルギー保存
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		//金属に吸収される光
		kD *= 1.0 - metallic;

		vec3 nom = D*F*G;

		float denom = 4.0*max(dot(Normal,V),0.0)*max(dot(Normal,L),0.0);

		//鏡面反射光(０割り防止で0.0001)
		vec3 specular = nom/max(denom,0.0001);

		//放射輝度(output)
		float NdotL = max(dot(Normal,L),0.0);
		Lo += (kD*albedo/PI + specular)*radiance*NdotL;

	}

	vec3 ambient = vec3(0.03)*albedo*ao;
	vec3 color = ambient+ Lo;

	//HDRマッピング
	color = color/(color + vec3(1.0));
	//ガンマ補正
	color = pow(color,vec3(1.0/2.2));

	fragment = vec4(color,1.0f);
}