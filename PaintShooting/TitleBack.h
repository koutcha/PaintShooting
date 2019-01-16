#pragma once
#include <memory>
#include <memory>
#include "Vector3.h"
class TextureRenderer2D;
class TitleBackAttributes;
class Shader;
class Texture;
class Window;
class TitleBack
{
public:
	TitleBack(const Vector3f& color0,const Vector3f& color1);
	~TitleBack();
	const std::array<Vector3f, 2> getColors()const;
	void draw(const TextureRenderer2D& renderer,const Window& window);
private:
	std::unique_ptr<TitleBackAttributes> attributes;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> texture;
};

