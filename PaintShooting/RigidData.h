#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class RigidData
{
public:
	RigidData();
	RigidData(const Vector3f& position, const Vector3f& scale, const Quaternion& rotation);
	~RigidData();

	void setPosition(const Vector3f&);
	void setScale(const Vector3f&);
	void setRotation(const Quaternion&);

	void setVelocity(const Vector3f&);

	const Vector3f getPosition()const;
	const Vector3f getScale()const;
	const Quaternion getRotation()const;
	const Vector3f getVelocity()const;
	
private:
	Vector3f position;
	Vector3f scale;
	Quaternion rotation;

	Vector3f velocity;
};

