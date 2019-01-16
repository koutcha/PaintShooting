#version 330

out vec4 fragment;

in vec3 P;
in vec2 UV;
in vec2 WUV;
flat in int TeamIndex;

uniform sampler2D paintMark;
uniform sampler2D bufferMask;

void main()
{
	vec2 wUV = WUV/2.0 + 0.5;
	vec4 sampled = texture(paintMark,UV);

	vec4 bufferSample = texture(bufferMask,wUV);
	if(sampled.a < 0.01 || bufferSample.b >0)
	{
		discard;
	}

	//sampled = TeamIndex == 0 ? sampled*vec4(1.0,0.0,0.0,1.0) : sampled*vec4(0.0,1.0,0.0,1.0);
	sampled = TeamIndex == 0 ? vec4(1.0,0.0,0.0,1.0)*sampled + vec4(bufferSample.r,0,0,bufferSample.a) : vec4(0.0,1.0,0.0,1.0)*sampled + vec4(0,bufferSample.g,0,1.0);


	fragment = vec4(sampled);

}
