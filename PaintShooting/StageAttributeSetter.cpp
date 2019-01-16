#include "StageAttributeSetter.h"
#include "GL/glew.h"
#include "Shader.h"
#include "PaintBufferTexture.h"
#include "PaintColorSet.h"
#include "Vector3.h"



StageAttributeSetter::StageAttributeSetter(const std::shared_ptr<PaintBufferTexture>& paintBufferTexture):
	texture(paintBufferTexture),
	colorset(std::make_shared<PaintColorSet>(Vector3f(1.0,0.0,0.0),Vector3f(0.0,0.0,1.0))),
	metallic(0.5f),
	roughness(0.4f),
	ao(1.0f)
{

}

StageAttributeSetter::StageAttributeSetter(const std::shared_ptr<PaintBufferTexture>& paintBufferTexture, const std::shared_ptr<PaintColorSet>& colorSet, float metallic, float roughness, float ao):
	texture(paintBufferTexture),
	colorset(colorSet),
	metallic(metallic),
	roughness(roughness),
	ao(ao)
{
}

StageAttributeSetter::~StageAttributeSetter()
{
}

void StageAttributeSetter::setAttributes(const Shader &shader) const
{
	texture->bind(8);
	glUniform1i(shader.getUniformLoc("paintMap"), 8);
	
	glUniform3fv(shader.getUniformLoc("redTexcelColor"), 1, colorset->getColor(0).getArray().data());
	glUniform3fv(shader.getUniformLoc("greenTexcelColor"), 1, colorset->getColor(1).getArray().data());

	glUniform1f(shader.getUniformLoc("paintMetallic"), metallic);
	glUniform1f(shader.getUniformLoc("paintRoughness"), roughness);
	glUniform1f(shader.getUniformLoc("paintAo"), ao);
}

void StageAttributeSetter::unbindAttributes() const
{
	texture->unbind(8);
}
