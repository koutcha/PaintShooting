#version 330
out vec4 fragment;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265369;

void main()
{
	//半球の中心から頂点に伸びるベクトル
	vec3 normal = normalize(localPos);
	//放射照度
	vec3 irradiance = vec3(0.0);
	//フラグメントの接空間の正規直交基底
	vec3 up = vec3(0.0,1.0,0.0);
	vec3 right = cross(up,normal);
	up = cross(normal,right);
	
	//sampleDelta幅で半球についての積分を離散的に近似する
	float sampleDelta = 0.025;
	//サンプリング数
	float nrSamples = 0.0;
	//単位球座標で和を求める 
	for(float phi = 0.0; phi<2.0*PI; phi+= sampleDelta)
	{
		for(float theta = 0.0; theta<0.5*PI; theta += sampleDelta)
		{
			//接空間でのサンプリングベクトル
			vec3 tangentSample = vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			//接空間からワールド空間へ変換
			vec3 sampleVec = tangentSample.x*right+tangentSample.y*up+tangentSample.z*normal;

			
			//上に行くほど面積が小さくなること、法線との角度が増えると減衰することを考慮する
			irradiance += texture(environmentMap,sampleVec).rgb*cos(theta)*sin(theta);
			nrSamples++;
		}
	
	}
	//平均して単位面積当たりのirradianceを求めて、面積をかけるとこの式になる
	irradiance = PI*irradiance*(1.0/nrSamples);

	//法線n方向のフラグメントに対する放射照度をテクスチャにレンダリングする
	fragment = vec4(irradiance,1.0);
}
