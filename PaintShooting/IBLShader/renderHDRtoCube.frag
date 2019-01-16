#version 330

out vec4 fragment;
in vec3 localPos;

//正距円筒図法マップ(2*PI<x<2*PI,-PI<y<PI)
uniform sampler2D equirectanglarMap;
//0.1591 = 1/2*PI 0.3183 = 1/3*PI
const vec2 invAtan = vec2(0.1591,0.3183);

//ベクトルからSphericalMapの座標へ返還
//テクスチャデータの反転はここのため
vec2 sampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
	//中心を0.5,0.5に変換
    uv += 0.5;
    return uv;
};

void main()
{		
	//座標変換して色格納
    vec2 uv = sampleSphericalMap(normalize(localPos));
    vec3 color = texture(equirectanglarMap, uv).rgb;
    fragment = vec4(color, 1.0);
}