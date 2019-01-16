#include "IBLRenderPack.h"

#include "HDRCubeMap.h"
#include "IrradianceMap.h"

#include "PrefilterMap.h"
#include "LUTTextureForIBL.h"




IBLRenderPack::IBLRenderPack(const std::string & hdrImagePath)
{
	cubeMap = std::make_unique<HDRCubeMap>(hdrImagePath);
	irradianceMap = std::make_unique<IrradianceMap>(*cubeMap);
	prefilterMap = std::make_unique<PrefilterMap>(*cubeMap);
	lut = std::make_unique<LUTTextureForIBL>();

}

IBLRenderPack::~IBLRenderPack()
{
}

void IBLRenderPack::bindIrradiace(unsigned int unit) const
{
	irradianceMap->bind(unit);
}

void IBLRenderPack::bindPrefilter(unsigned int unit) const
{
	prefilterMap->bind(unit);
}

void IBLRenderPack::bindBRDFLut(unsigned int unit) const
{
	lut->bind(unit);
}

void IBLRenderPack::bindEnvironment(unsigned int unit) const
{
	cubeMap->bind(unit);
}

void IBLRenderPack::unbindIrradiance(unsigned int unit) const
{
	irradianceMap->unbind(unit);
}

void IBLRenderPack::unbindPrefilter(unsigned int unit) const
{
	prefilterMap->unbind(unit);
}

void IBLRenderPack::unbindBRDFLut(unsigned int unit) const
{
	lut->unbind(unit);
}

void IBLRenderPack::unbindEnvironment(unsigned int unit) const
{
	cubeMap->unbind(unit);
}
