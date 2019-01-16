#pragma once
#include <memory>
#include <vector>

class Bullet;
class StageData;
class StagePainting;
class CharacterPhysics;
class StageObstacle;

struct PaintBullet;

class StagePhysics
{
public:
	StagePhysics();
	void setStagePainting(const std::shared_ptr<StagePainting>& painter);
	void setStageData(const std::shared_ptr<StageData>& data);
	~StagePhysics();
	bool collisionBullet(const PaintBullet&);
	//get Y-Height point (x,z)
	float getHeight(float x, float z)const;
	float getPlaneWidth()const;

	const std::vector<StageObstacle>& getObstacles()const;

private:
	std::shared_ptr<StagePainting> painter;
	std::shared_ptr<StageData> data;
};

