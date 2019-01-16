#include "TitleBack.h"
#include "TitleBackAttributes.h"
#include "TextureRenderer2D.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"

TitleBack::TitleBack(const Vector3f & color0, const Vector3f & color1):
	attributes(std::make_unique<TitleBackAttributes>(color0,color1)),
	shader(std::make_unique<Shader>("Shader\\textureRenderer.vert","Shader\\titleback.frag")),
	texture(std::make_unique<Texture>("Resource\\Image\\titleback.png"))
{

}

TitleBack::~TitleBack()
{
}

const std::array<Vector3f, 2> TitleBack::getColors() const
{
	return attributes->getColors();
}

void TitleBack::draw(const TextureRenderer2D & renderer, const Window & window)
{
	float width = window.getSize()[0];
	float height = window.getSize()[1];
	texture->bind(0);
	renderer.renderTexture(0, 0, 0, width, height, width, height,*shader,*attributes);
	texture->unbind(0);
}

