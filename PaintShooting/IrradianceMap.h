#pragma once
#include <string>
class HDRCubeMap;
class IrradianceMap
{
public:
	IrradianceMap(const HDRCubeMap&);
	~IrradianceMap();
	void bind(unsigned int unit)const;
	void unbind(unsigned int unit)const;
private:
	static const std::string shaderPath;
	unsigned int irradianceMap;

};

