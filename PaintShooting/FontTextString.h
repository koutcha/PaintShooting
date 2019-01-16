#pragma once
#include "FontText.h"
#include <string>
class FontTextString:
	public FontText
{
public:
	FontTextString(const Vector2f& position, float scale, const Vector3f& color, const std::string& text);
	~FontTextString();
	void setText(const std::string& str);
	const std::string& getText()const;

	// FontText ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void draw(const Font &font, float windowWidth, float windowHeight) override;
private:
	std::string text;

};

