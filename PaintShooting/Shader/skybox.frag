#version 330
out vec4 fragment;

in vec3 localPos;
  
uniform samplerCube environmentMap;
uniform float exposure;
void main()
{
    //vec3 envColor = texture(environmentMap, localPos).rgb;

    vec3 envColor = textureLod(environmentMap,localPos, 0.0).rgb; 
	//hdr tonemapping
    envColor = vec3(1.0) - exp(-envColor*exposure);
	//gamma correct
    envColor = pow(envColor, vec3(1.0/2.2)); 

    fragment = vec4(envColor,1.0);
}