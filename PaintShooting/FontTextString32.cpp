#include "FontTextString32.h"
#include "Font.h"
FontTextString32::FontTextString32():
	FontText(),
	text()
{
}
FontTextString32::FontTextString32(const Vector2f & position, float scale, const Vector3f & color, const std::u32string & text):
	FontText(position,scale,color),
	text(text)
{
	std::cout << text.c_str();
}

FontTextString32::~FontTextString32()
{
}

void FontTextString32::setText(const std::u32string &text)
{
	this->text = text;
}

const std::u32string & FontTextString32::getText() const
{
	return text;
}

void FontTextString32::draw(const Font &font, float windowWidth, float windowHeight)
{
	Vector2f postiion = getPosition();
	font.renderText(text, postiion.x, postiion.y, getScale(), getColor(), windowWidth, windowHeight);
}
