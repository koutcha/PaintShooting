#pragma once
#include<memory>
#include <string>
class Shader;
class AdditionalAttributeSetter;
class TextureRenderer2D
{
public:
	TextureRenderer2D();
	~TextureRenderer2D();

	void renderTexture(unsigned int textureUnit, float x, float y, float width, float height, float windowWidth, float windowHeight)const;
	void renderTexture(unsigned int textureUnit, float x, float y, float width, float height, float windowWidth, float windowHeight,const Shader& subShader,const  AdditionalAttributeSetter& setter)const;
	void renderTextureNotBlending(unsigned int textureUnit, float x, float y, float width, float height, float windowWidth, float windowHeight)const;

private:
	unsigned int vao;
	unsigned int vbo;
	static const std::string shaderPath;
	std::unique_ptr<Shader> shader;

};

