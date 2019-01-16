#pragma once
#include "Vector2.h"
#include "Vector3.h"
class Font;
class FontText
{
public:
	FontText();
	FontText(const Vector2f& position,float scale,const Vector3f& color);
	virtual ~FontText();
	virtual void draw(const Font&,float windowWidth,float windowHeight) = 0;
	//テキストの左下位置
	void setPosition(float x, float y);
	//テキストの左下位置
	void setPosition(const Vector2f& position);

	//画面中心(0,0) ,x(-1 ~ 1),y(-1~1)としたときのテキストの左下位置を計算して格納する
	//引数には-1 ~1までの範囲で入力
	void setScreenPosition(float x, float y,float windowWidth,float windowHeight);
	void setScale(float scale);
	void setColor(const Vector3f& color);
	//テキストの左下位置
	const Vector2f getPosition()const;
	//画面中心(0,0) ,x(-1 ~ 1),y(-1~1)としたときのテキストの左下位置を計算して返す
	const Vector2f getScreenPosition(float windowWidth,float windowHeight)const;
	float getScale()const;
	const Vector3f getColor()const;
private:
	Vector2f position;
	float scale;
	Vector3f color;
};

