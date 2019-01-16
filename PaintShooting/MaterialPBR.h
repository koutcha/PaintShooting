#pragma once
#include "Vector3.h"
class MaterialPBR
{
public:
	MaterialPBR();
	MaterialPBR(const Vector3f& albedo,float roughness,float metallic,float ao);
	~MaterialPBR();

	void setAlbedo(const Vector3f& value);
	void setRoughness(float value);
	void setMetallic(float value);
	void setAo(float value);

	const Vector3f getAlbedo()const;
	const float getRoughness()const;
	const float getMetallic()const;
	const float getAo()const;
	
private:
	Vector3f albedo;
	float roughness;
	float metallic;
	float ao;

};

