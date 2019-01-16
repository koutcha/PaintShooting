#include "PaintColorSet.h"



PaintColorSet::PaintColorSet(const Vector3f & color0, const Vector3f& color1):
	colorData({color0,color1})
{
}

PaintColorSet::~PaintColorSet()
{
}

const Vector3f PaintColorSet::getColor(int teamIndex)const
{
	if (teamIndex > 1 || teamIndex < 0)
	{
		return Vector3f(0, 0, 0);
	}

	return colorData[teamIndex];
}


