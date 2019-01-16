#pragma once
#include <vector>
#include <array>
#include "Vector3.h"

class Shape;
class GraphicIBL;
class StageObstacle;
class StageData
{
public:
	StageData(float scale);
	StageData(float scale,float wallHeight,int obstaclesNumber);
	~StageData();
	float getScale()const;
	void setScale(float scale);
	float getWallHeight()const;
	void setWallHeight(float scale);

	std::vector<StageObstacle>& getObstales();
private:
	float scale;
	float wallHeight;

	std::vector<StageObstacle> stageObstacles;
};

