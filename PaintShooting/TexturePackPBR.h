#pragma once
#include <string>
#include <array>
#include "Texture.h"
class TexturePackPBR
{
public:
	//フォルダのパス、拡張子はpngで統一(albedo,normal,metallic,roughness,ao)
	TexturePackPBR(const std::string& folderPath);
	//それぞれのテクスチャごとのファイルパスを引数に
	TexturePackPBR(const std::string& albedo,const std::string& normal,const std::string& metallic,const std::string&roughness,const std::string& ao);
	~TexturePackPBR();

	void bindAlbedo(unsigned int unit)const;
	void bindNormal(unsigned int unit)const;
	void bindMetallic(unsigned int unit)const;
	void bindRoughness(unsigned int unit)const;
	void bindAo(unsigned int unit)const;

	void unbindAlbedo(unsigned int unit)const;
	void unibindNormal(unsigned int unit)const;
	void unbindMetallic(unsigned int unit)const;
	void unbindRoughness(unsigned int unit)const;
	void unbindAo(unsigned int unit)const;

private:
	static const int attributeNumber = 5;
	std::array<Texture, attributeNumber> textures;
};

