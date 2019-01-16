#pragma once
#include "AdditionalAttributeSetter.h"
#include <memory>
#include <array>
class PaintBufferTexture;
class PaintColorSet;
class StageAttributeSetter :
	public AdditionalAttributeSetter
{
public:
	StageAttributeSetter(const std::shared_ptr<PaintBufferTexture>& paintBufferTexture);
	StageAttributeSetter(const std::shared_ptr<PaintBufferTexture>& paintBufferTexture,const std::shared_ptr<PaintColorSet>& colorSet,float metallic,float roughness,float ao);

	~StageAttributeSetter();

	// AddtionalAttributeSetter ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void setAttributes(const Shader& shader) const override;
	void unbindAttributes() const override;
private:
	std::shared_ptr<PaintBufferTexture> texture;
	std::shared_ptr<PaintColorSet> colorset;
	float metallic;
	float roughness;
	float ao;
};

