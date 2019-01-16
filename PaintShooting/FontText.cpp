#include "FontText.h"



FontText::FontText():
	position(0.0f,0.0f),
	scale(1.0f),
	color(0.0f,0.0f,0.0f)
{
}

FontText::FontText(const Vector2f & position, float scale, const Vector3f & color):
	position(position),
	scale(scale),
	color(color)
{

}


FontText::~FontText()
{
}

void FontText::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void FontText::setPosition(const Vector2f & position)
{
	this->position = position;
}

void FontText::setScreenPosition(float x, float y, float windowWidth, float windowHeight)
{
	x = (x + 1.0f)*windowWidth;
	y = (y + 1.0f)*windowHeight;

	this->position.x = x;
	this->position.y = y;
}

void FontText::setScale(float scale)
{
	this->scale = scale;
}

void FontText::setColor(const Vector3f & color)
{
	this->color = color;
}

const Vector2f FontText::getPosition() const
{
	return position;
}

const Vector2f FontText::getScreenPosition(float width,float height) const
{
	Vector2f result;
	result.x = position.x / width;
	result.y = position.y / height;
	return result;
}

float FontText::getScale() const
{
	return scale;
}

const Vector3f FontText::getColor() const
{
	return color;
}
