#pragma once
#include <array>
#include "Vector3.h"
class PaintColorSet
{
public:
	PaintColorSet(const Vector3f& color0,const Vector3f& color1);
	~PaintColorSet();
	const Vector3f getColor(int teamIndex)const;
private:
	std::array<Vector3f, 2> colorData;
};

