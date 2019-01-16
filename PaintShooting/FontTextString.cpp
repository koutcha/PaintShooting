#include "FontTextString.h"
#include "Font.h"

FontTextString::FontTextString(const Vector2f & position, float scale, const Vector3f & color, const std::string & text):
	FontText(position,scale,color),
	text(text)
{
}

FontTextString::~FontTextString()
{
}

void FontTextString::setText(const std::string & str)
{
	this->text = str;
}

const std::string & FontTextString::getText() const
{
	return text;
}

void FontTextString::draw(const Font &font, float windowWidth, float windowHeight)
{
	Vector2f position = getPosition();
	font.renderText(text,position.x,position.y,getScale(),getColor(),windowWidth,windowHeight);
}
