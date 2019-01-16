#version 330

//頂点情報
in vec3 P;
in vec3 N;
in vec2 UV;
in vec3 T;

out vec4 fragment;

//視点位置
uniform vec3 viewPos;

//テクスチャon off
uniform bool useMaterialTexture;

//直接光(ダイレクトライティング)
//ライトの数
const int MAX_LIGHT = 4;

//ライトの位置、色、強さ
uniform vec3 PLpos[MAX_LIGHT];
uniform vec3 PLcolor[MAX_LIGHT];
uniform float PLintensity[MAX_LIGHT];

//hdr補正値
uniform float exposure;

//間接光(画像ベースライティング)

uniform sampler2D brdfLUT;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;

const float PI = 3.1415926;

//材質データ

//数値型
uniform vec3 albedoMaterial;
uniform float roughnessMaterial;
uniform float metallicMaterial;
uniform float aoMaterial;

//テクスチャ型
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;




vec3 getNormalFromMap()
{
	//テクスチャ座標の法線
	vec3 tangentNormal = texture(normalMap,UV).rgb*2.0 - 1.0;

	//接空間を求める
	vec3 Normal = normalize(N);
	vec3 Tangent = normalize(T);
	vec3 Bitangent = cross(Normal,Tangent);
	//ワールド空間へ変換
	mat3 TBN = mat3(Tangent,Bitangent,Normal);

	return normalize(TBN*tangentNormal);
}


//法線分布関数D
//ハーフベクトルに影響するように整列されているマイクロファセットの量を近似する
//マイクロファセットが面の法線にどのようにずれて分布しているかを統計的に求める
//マイクロファセット近似で最も重要な部分
//N:法線　H:ハーフベクトル　a 粗さ
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

//幾何関数G
//マイクロファセットによって視線から隠れる部分や光が届かず陰になる部分を記述する
//表面が粗いときにマイクロファセットが他のマイクロファセットに影を落とし、結果として表面の反射光が減少する
//また反射した光が他のマイクロファセットい遮られて視線に届かない場合もある

//NdotV(NdotL)　内積　k:roughnessから求める定数項
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

	//視線からの遮蔽
	float ggx1 = GeometrySchlickGGX(NdotV,roughness);
	//光源からの遮蔽
	float ggx2 = GeometrySchlickGGX(NdotL,roughness);

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
		//放射輝度の計算(input)
		vec3 L = normalize(PLpos[i]  - P);
		vec3 H = normalize(V+L);

		float d = length(PLpos[i] - P);
		float attenuation = 1.0/ (d*d);

		vec3 radiance = PLcolor[i]*PLintensity[i]*attenuation;

		//クックトランスbrdf
		float D = DistributionGGX(Normal,H,roughness);
		float G = GeometrySmith(Normal,V,L,roughness);
		//max(dot(H,V),0.0)ではなくclamp(dot(H,V),0.0,1.0);
		vec3 F = fresnelSchlick(clamp(dot(H,V),0.0,1.0),F0);
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

		


	//環境マップの間接光についての計算を追加、環境光の計算を変更
	vec3 F = fresnelSchlickRoughness(max(dot(Normal, V), 0.0), F0, roughness); 
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= (1.0 - metallic);	


	vec3 irradiance = texture(irradianceMap, Normal).rgb;
	vec3 diffuse  = irradiance *albedo;

	//環境マップからの鏡面反射の計算
	const float MAX_REFLECTION_LOD = 4.0;
	//Lod忘れないで
	vec3 prefilteredColor = textureLod(prefilterMap,R,roughness*MAX_REFLECTION_LOD).rgb;
	vec2 envBRDF = texture(brdfLUT,vec2(max(dot(Normal,V),0.0),roughness)).rg;
	//スペキュラの積分計算式
	vec3 specular =  prefilteredColor*(F*envBRDF.x+envBRDF.y);

	vec3 ambient    = (kD * diffuse+specular) * ao;
	//Loが原因で黒点ノイズが発生
	vec3 color =ambient + Lo;


	//HDRマッピング
	color = vec3(1.0) - exp(-color*exposure);
	//ガンマ補正
	color = pow(color,vec3(1.0/2.2));

	fragment = vec4(color,1.0f);
}