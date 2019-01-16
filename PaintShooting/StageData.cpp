#include "StageData.h"
#include "StageObstacle.h"

StageData::StageData(float scale):
	scale(scale),
	wallHeight(100.0),
	stageObstacles(0)
{

}



StageData::StageData(float scale,float wallHeight,int obstaclesNumber):
	scale(scale),
	wallHeight(wallHeight),
	stageObstacles(obstaclesNumber)
{
	
}

StageData::~StageData()
{
}

float StageData::getScale()const
{
	return scale;
}

void StageData::setScale(float scale)
{
	this->scale = scale;
}

float StageData::getWallHeight() const
{
	return wallHeight;
}

void StageData::setWallHeight(float height)
{
	wallHeight = height;
}


std::vector<StageObstacle>& StageData::getObstales()
{
	return stageObstacles;
}
