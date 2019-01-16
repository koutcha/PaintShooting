#include "MaterialPBR.h"



MaterialPBR::MaterialPBR():
	albedo(1.0f,1.0f,1.0f),
	roughness(0.5f),
	metallic(0.5f),
	ao(1.0f)
{
}

MaterialPBR::MaterialPBR(const Vector3f & albedo, float roughness, float metallic, float ao):
	albedo(albedo),
	roughness(roughness),
	metallic(metallic),
	ao(ao)
{
}


MaterialPBR::~MaterialPBR()
{
}

void MaterialPBR::setAlbedo(const Vector3f & value)
{
	albedo = value;
}

void MaterialPBR::setRoughness(float value)
{
	roughness = value;
}

void MaterialPBR::setMetallic(float value)
{
	metallic = value;
}

void MaterialPBR::setAo(float value)
{
	ao = value;
}

const Vector3f MaterialPBR::getAlbedo() const
{
	return albedo;
}

const float MaterialPBR::getRoughness() const
{
	return roughness;
}

const float MaterialPBR::getMetallic() const
{
	return metallic;
}

const float MaterialPBR::getAo() const
{
	return ao;
}
