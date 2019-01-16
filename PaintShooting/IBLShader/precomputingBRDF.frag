#version 330
out vec2 fragment;
in vec2 UV;
//BRDFの積分結果を事前計算する

const float PI = 3.14159265359;


//準モンテカルロ法のための規則的であるが一様な低食い違い量列を生成する
//乱数よりも厳密性は下がるが解への収束が早い

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

//表面の粗さに対してサンプリングするマイクロサーフェスまわりのハーフベクトルの配向を決める(サンプリングベクトルを返す)
//粗ければ鏡面ローブ（ハーフベクトルの広がり方）が広くなる
vec3 ImportanceSampleGGX(vec2 Xi,vec3 N,float roughness)
{
	float a = roughness*roughness;

	float phi = 2.0*PI*Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0)*Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	//球座標からデカルト座標系に変換
	vec3 H;
	H.x = cos(phi)*sinTheta;
	H.y = cos(phi)*sinTheta;
	H.z = cosTheta;

	//接空間からワールド座標系に変換
	//基底行列のランクが減らないようにupベクトルを決める
	vec3 up =abs(N.z) < 0.999?vec3(0.0,0.0,1.0):vec3(1.0,0.0,0.0);
	vec3 tangent = normalize(cross(up,N));
	vec3 bitangent = cross(N,tangent);
	vec3 sampleVec = tangent*H.x + bitangent*H.y + N*H.z;

	return normalize(sampleVec);
}

//幾何関数G　マイクロファセットで遮断される光線の近似
float GeometrySchlickGGX(float NdotV,float roughness)
{
	float a = roughness;

	//IBLの場合はk = (roughnes+1)^2/8ではなくk= roughness^2/2
	float k = (a*a)/2.0;
	float nom = NdotV;
	float denom = NdotV*(1.0 - k)+k;
	return nom/denom;
}
//入射と反射両方で起こる遮断を考慮する
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness)
{
	//反射
	float NdotV = max(dot(N,V),0.0);
	float ggx2 = GeometrySchlickGGX(NdotV,roughness);
	//入射
	float NdotL = max(dot(N,L),0.0);
	float ggx1 = GeometrySchlickGGX(NdotL,roughness);

	return ggx1*ggx2;
}


//NdotVを横軸、roughnessを縦軸に取り事前計算結果をテクスチャに焼き付ける
vec2 IntegrateBRDF(float NdotV,float roughness)
{
	//恐らくNとVのなす角(NdotV)しか問題にしないのでy方向の成分について考える必要がない
	vec3 V;
	V.x = sqrt(1.0 - NdotV*NdotV);
	V.y = 0.0;
	V.z = NdotV;

	float A = 0.0;
	float B = 0.0;
	//Nは固定する
	vec3 N = vec3(0.0,0.0,1.0);
	
	const uint SAMPLE_COUNT = 1024u;
	for(uint i=0u; i<SAMPLE_COUNT; ++i)
	{
		vec2 Xi = Hammersley(i,SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi,N,roughness);
		vec3 L = normalize(2.0*dot(V,H)*H-V);
		//N=(0.0,0.0,1.0)であることにより内積はZ成分の値と一致する
		float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

		if(NdotL>0.0)
		{
			//BRDFの計算
			float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);
			//積分の式変形で式を2項に分ける、第一項をA、第二項をBにそれぞれ格納する
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