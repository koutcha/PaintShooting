#include "RigidData.h"



RigidData::RigidData():
	position(0,0,0),
	scale(1.0,1.0,1.0),
	velocity(0,0,0)
{
	rotation.setRotation(Vector3f(0, 1, 0), 0.0f);
}

RigidData::RigidData(const Vector3f & position, const Vector3f & scale, const Quaternion & rotation):
	velocity(0,0,0),
	position(position),
	scale(scale),
	rotation(rotation)
{

}


RigidData::~RigidData()
{
}

void RigidData::setPosition(const Vector3f &position)
{
	this->position = position;
}

void RigidData::setScale(const Vector3f &scale)
{
	this->scale = scale;
}

void RigidData::setRotation(const Quaternion &rotation)
{
	this->rotation = rotation;
}

void RigidData::setVelocity(const Vector3f &velocity)
{
	this->velocity = velocity;
}

const Vector3f RigidData::getPosition() const
{
	return position;
}

const Vector3f RigidData::getScale() const
{
	return scale;
}

const Quaternion RigidData::getRotation() const
{
	return rotation;
}

const Vector3f RigidData::getVelocity() const
{
	return velocity;
}




