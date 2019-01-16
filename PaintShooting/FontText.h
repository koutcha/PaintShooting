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
	//�e�L�X�g�̍����ʒu
	void setPosition(float x, float y);
	//�e�L�X�g�̍����ʒu
	void setPosition(const Vector2f& position);

	//��ʒ��S(0,0) ,x(-1 ~ 1),y(-1~1)�Ƃ����Ƃ��̃e�L�X�g�̍����ʒu���v�Z���Ċi�[����
	//�����ɂ�-1 ~1�܂ł͈̔͂œ���
	void setScreenPosition(float x, float y,float windowWidth,float windowHeight);
	void setScale(float scale);
	void setColor(const Vector3f& color);
	//�e�L�X�g�̍����ʒu
	const Vector2f getPosition()const;
	//��ʒ��S(0,0) ,x(-1 ~ 1),y(-1~1)�Ƃ����Ƃ��̃e�L�X�g�̍����ʒu���v�Z���ĕԂ�
	const Vector2f getScreenPosition(float windowWidth,float windowHeight)const;
	float getScale()const;
	const Vector3f getColor()const;
private:
	Vector2f position;
	float scale;
	Vector3f color;
};

