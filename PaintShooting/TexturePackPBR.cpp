#include "TexturePackPBR.h"


TexturePackPBR::TexturePackPBR(const std::string & folderPath)
{
	textures[0].reset(folderPath + "\\albedo.png");
	textures[1].reset(folderPath + "\\normal.png");
	textures[2].reset(folderPath + "\\metallic.png");
	textures[3].reset(folderPath + "\\roughness.png");
	textures[4].reset(folderPath + "\\ao.png");

}

TexturePackPBR::TexturePackPBR(const std::string & albedo, const std::string & normal, const std::string & metallic, const std::string& roughness, const std::string& ao)
{
	textures[0].reset(albedo);
	textures[1].reset(normal);
	textures[2].reset(metallic);
	textures[3].reset(roughness);
	textures[4].reset(ao);
}

TexturePackPBR::~TexturePackPBR()
{
}

void TexturePackPBR::bindAlbedo(unsigned int unit) const
{
	textures[0].bind(unit);
}

void TexturePackPBR::bindNormal(unsigned int unit) const
{
	textures[1].bind(unit);

}

void TexturePackPBR::bindMetallic(unsigned int unit) const
{
	textures[2].bind(unit);

}

void TexturePackPBR::bindRoughness(unsigned int unit) const
{
	textures[3].bind(unit);

}

void TexturePackPBR::bindAo(unsigned int unit) const
{
	textures[4].bind(unit);

}

void TexturePackPBR::unbindAlbedo(unsigned int unit) const
{

	textures[0].unbind(unit);
}

void TexturePackPBR::unibindNormal(unsigned int unit) const
{
	textures[1].unbind(unit);

}

void TexturePackPBR::unbindMetallic(unsigned int unit) const
{
	textures[2].unbind(unit);

}

void TexturePackPBR::unbindRoughness(unsigned int unit) const
{
	textures[3].unbind(unit);

}

void TexturePackPBR::unbindAo(unsigned int unit) const
{
	textures[4].unbind(unit);

}
