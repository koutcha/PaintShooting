#pragma once
#include "Vector3.h"
class RigidData;
class CollisionSphere
{

public:
	CollisionSphere(const Vector3f& position,float Radius);
	//rigidataから生成
	//半径データはscaleの最大値とする
	CollisionSphere(const RigidData&);
	~CollisionSphere();
	void setPosition(const Vector3f&);
	void setRadius(float radius);
	const Vector3f  getPosition()const;
	float getRadius()const;
	bool isCollision(const CollisionSphere& target);
	//if (0,0,0), not sink
	const Vector3f getVectorModifySinking(const CollisionSphere& target);

private:
	Vector3f position;
	float radius;
};

