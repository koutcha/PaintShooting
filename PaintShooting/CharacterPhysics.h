#pragma once
#include <memory>
#include "Vector3.h"
class RigidData;
class PaintActorData;
class Bullet;
class StagePhysics;
class Bomb;
struct PaintBullet;
class CharacterPhysics
{
public:
	CharacterPhysics();
	~CharacterPhysics();
	void setMaxVelocityLength(float maxVelocity);
	void setRigid(const std::shared_ptr<RigidData>&);
	void setActorData(const std::shared_ptr<PaintActorData>&);
	void update(float dt, float gravity);
	void collision(const StagePhysics& stage);
	void collision(const Bomb& bomb);
	bool checkCollision(const PaintBullet&);

private:
	float maxVelocity;
	std::shared_ptr<RigidData> rigid;
	std::shared_ptr<PaintActorData> data;
};

