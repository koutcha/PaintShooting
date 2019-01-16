#include "PointLights.h"


PointLightData::PointLightData():
	position(0.0f,0.0f,0.0f),
	color(0.0f,0.0f,0.0f),
	intensity(0.0f)
{

}

PointLightData::PointLightData(const Vector3f &position, const Vector3f &color, float intensity):
	position(position),
	color(color),
	intensity(intensity)
{

}


PointLights::PointLights()
{
	positions.fill(0.0f);
	colors.fill(0.0f);
	intensities.fill(0.0f);
}


PointLights::~PointLights()
{
}

void PointLights::setData(int index, const PointLightData &data)
{
	if (index<0 || index>=MAX_LIGHT_NUMBER)
	{
		//do not set out of bound
	}
	positions[index * 3] = data.position.x;
	positions[index * 3 + 1] = data.position.y;
	positions[index * 3 + 2] = data.position.z;

	colors[index * 3] = data.color.x;
	colors[index * 3 + 1] = data.color.y;
	colors[index * 3 + 2] = data.color.z;

	intensities[index] = data.intensity;

}

const std::array<float,PointLights::MAX_LIGHT_NUMBER * 3>& PointLights::getPositionData() const
{
	return positions;
}

const std::array<float, PointLights::MAX_LIGHT_NUMBER * 3>& PointLights::getColorData() const
{
	return colors;
}

const std::array<float, PointLights::MAX_LIGHT_NUMBER>& PointLights::getIntensityData() const
{
	return intensities;
}

