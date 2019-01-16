#version 330

out vec4 fragment;

in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

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
	float cosTheta = sqrt( (1.0 - Xi.y)  /  (1.0 + (a*a - 1.0) * Xi.y  )  );
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	//球座標からデカルト座標系に変換
	vec3 H;
	H.x = cos(phi)*sinTheta;
	H.y = sin(phi)*sinTheta;
	H.z = cosTheta;

	//接空間からワールド座標系に変換
	//基底行列のランクが減らないようにupベクトルを決める
	vec3 up =abs(N.z) < 0.999?vec3(0.0,0.0,1.0):vec3(1.0,0.0,0.0);
	vec3 tangent = normalize(cross(up,N));
	vec3 bitangent = cross(N,tangent);
	vec3 sampleVec = tangent*H.x + bitangent*H.y + N*H.z;

	return normalize(sampleVec);
}

void main()
{
	//N=Vと仮定する(事前計算のため視線ベクトルに依存しないようにする）
	vec3 N = normalize(localPos);
	vec3 R = N;
	vec3 V = R;
	//サンプリングするベクトル数
	const uint SAMPLE_COUNT = 1024u;
	float totalWeight = 0.0;
	vec3 prefilteredColor = vec3(0.0);

	//準モンテカルロ積分
	//sumを求める
	for(uint i=0u; i<SAMPLE_COUNT; ++i)
	{
		vec2 Xi = Hammersley(i,SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi,N,roughness);
		vec3 L = normalize(2.0*dot(V,H)*H-V);
		float NdotL = max(dot(N, L), 0.0);
		if(NdotL>0.0)
		{
			//↓リンクエラーの原因(texture関数) .rgbがないとダメらしい
			prefilteredColor += texture(environmentMap,L).rgb*NdotL;
			totalWeight += NdotL;
		}	
	}
	//影響の少ないサンプル(NdotLの小さい）がより寄与していない、最終重量（重みの合計）でprefiteredColorを割る
	//cosThetaすなわちNdotLで重み付けすることによりより良い結果が得られたらしい
	prefilteredColor = prefilteredColor/totalWeight;

	fragment = vec4(prefilteredColor,1.0);
}