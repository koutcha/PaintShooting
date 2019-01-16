#include "CollisionSphere.h"
#include "RigidData.h"



CollisionSphere::CollisionSphere(const Vector3f & position, float radius):
	position(position),
	radius(radius)
{
	if (radius < 0)
	{
		radius = 0;
	}
}

CollisionSphere::CollisionSphere(const RigidData &rigid):
	position(rigid.getPosition()),
	radius(0)
{
	//Šg‘å•ûŒü‚ÌÅ‘å’l‚ðŽæ“¾
	Vector3f scale = rigid.getScale();
	for (int i = 0; i < 3; i++)
	{
		if (radius < scale.getByIndex(i))
		{
			radius = scale.getByIndex(i);
		}
	}
}

CollisionSphere::~CollisionSphere()
{
}

void CollisionSphere::setPosition(const Vector3f &posiition)
{
	this->position = posiition;
}

void CollisionSphere::setRadius(float radius)
{
	this->radius = radius;
}

const Vector3f CollisionSphere::getPosition() const
{
	return position;
}

float CollisionSphere::getRadius() const
{
	return radius;
}

bool CollisionSphere::isCollision(const CollisionSphere & target)
{
	float distance = (target.getPosition() - this->position).length();

	return distance < this->radius + target.radius;
}

const Vector3f CollisionSphere::getVectorModifySinking(const CollisionSphere & target)
{
	Vector3f actionVector = target.getPosition() - this->position;
	float distance = actionVector.length();

	float sink = (this->radius + target.radius) - distance;
	if (sink > 0)
	{
		actionVector.normalize();
		actionVector *= sink;
		return actionVector;
	}
	else
	{
		return Vector3f(0.0f, 0.0f, 0.0f);
	}


}
