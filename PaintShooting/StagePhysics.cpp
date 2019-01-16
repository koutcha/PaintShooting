#include "StagePhysics.h"
#include "StagePainting.h"
#include "StageData.h"

#include "StageObstacle.h"
#include "PaintBullet.h"

StagePhysics::StagePhysics()
{
}

void StagePhysics::setStagePainting(const std::shared_ptr<StagePainting>& painter)
{
	this->painter = painter;
}

void StagePhysics::setStageData(const std::shared_ptr<StageData>& data)
{
	this->data = data;
}


StagePhysics::~StagePhysics()
{
}

bool StagePhysics::collisionBullet(const PaintBullet &bullet)
{
	Vector3f position = bullet.position;
	float scale = data->getScale();
	if (position.z < scale && position.x < scale && position.x >-scale && position.z > -scale)
	{
		if (position.y < 0)
		{
			painter->inputPos(bullet.position.x, bullet.position.z,bullet.paintScale, bullet.velocity, rand(),bullet.teamIndex);
			return true;
		}
		for (const auto & obstacle : data->getObstales())
		{
			if (obstacle.checkCollision(bullet.position,bullet.scale))
			{
				return true;
			}
		}

		return false;
	}
	else
	{
		return true;
	}

	return false;
}


float StagePhysics::getHeight(float x, float z) const
{
	if (x > data->getScale() || x < -data->getScale() ||
		z < -data->getScale() || z> data->getScale())
	{
		return data->getWallHeight();
	}
	return 0;
}

float StagePhysics::getPlaneWidth() const
{
	return data->getScale();
}

const std::vector<StageObstacle>& StagePhysics::getObstacles() const
{
	return data->getObstales();
}
