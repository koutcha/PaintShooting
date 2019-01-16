#pragma once
#include <array>
#include "Vector3.h"

struct PointLightData
{
	PointLightData();
	PointLightData(const Vector3f& position, const Vector3f& color, float intensity);
	Vector3f position;
	Vector3f color;
	float intensity;
};

class PointLights
{
public:
	static constexpr int MAX_LIGHT_NUMBER = 4;
	PointLights();
	~PointLights();

	void setData(int index,const PointLightData&);

	const std::array<float, MAX_LIGHT_NUMBER * 3>& getPositionData()const;
	const std::array<float, MAX_LIGHT_NUMBER * 3>& getColorData()const;
	const std::array<float, MAX_LIGHT_NUMBER>& getIntensityData()const;
private:
	//�ʒu
	std::array<float, MAX_LIGHT_NUMBER*3> positions;

	//color*intensity:�ŏI�I�Ȍ����x�A����т��Ȃ��悤�ɃV�F�[�_�[�ŕ␳����(HDR�␳)

	//�F
	std::array<float, MAX_LIGHT_NUMBER *3> colors;
	//���x
	std::array<float, MAX_LIGHT_NUMBER> intensities;
};

