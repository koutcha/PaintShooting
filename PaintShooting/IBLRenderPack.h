#pragma once
#include <memory>
#include <string>
class LUTTextureForIBL;
class PrefilterMap;
class HDRCubeMap;
class IrradianceMap;

class IBLRenderPack
{
public:
	IBLRenderPack(const std::string& hdrImagePath);
	~IBLRenderPack();
	
	void bindIrradiace(unsigned int unit)const;
	void bindPrefilter(unsigned int unit)const;
	void bindBRDFLut(unsigned int unit)const;

	void bindEnvironment(unsigned int unit)const;

	void unbindIrradiance(unsigned int unit)const;
	void unbindPrefilter(unsigned int unit)const;
	void unbindBRDFLut(unsigned int unit)const;
	
	void unbindEnvironment(unsigned int unit)const;
private:
	std::shared_ptr<HDRCubeMap> cubeMap;
	std::shared_ptr<IrradianceMap> irradianceMap;
	std::shared_ptr<PrefilterMap> prefilterMap;
	std::shared_ptr<LUTTextureForIBL> lut;

};

