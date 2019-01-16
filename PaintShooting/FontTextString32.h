#pragma once
#include "FontText.h"
#include <string>
class FontTextString32 :
	public FontText
{
public:
	FontTextString32();
	FontTextString32(const Vector2f& position,float scale,const Vector3f& color,const std::u32string& text);
	~FontTextString32();
	void setText(const std::u32string& text);
	const std::u32string& getText()const;
	// FontText ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void draw(const Font &font, float windowWidth, float windowHeight) override;
private:
	std::u32string text;


};

