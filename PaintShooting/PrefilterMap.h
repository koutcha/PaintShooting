#pragma once
#include <string>
class HDRCubeMap;
class PrefilterMap
{
public:
	PrefilterMap(const HDRCubeMap& envMap);
	~PrefilterMap();
	void bind(unsigned int unit) const;
	void unbind(unsigned int unit)const;
private:
	static const std::string shaderPath;
	unsigned int prefilterMap;
};

