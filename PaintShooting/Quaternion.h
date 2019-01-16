#pragma once
#include "Vector3.h"
class Quaternion
{
	friend class Matrix;
private:
	Vector3f v;
	float w;
public:
	Quaternion();
	Quaternion(const Vector3f & xyz, float w);
	~Quaternion();

	void setConjugate(const Quaternion & q);
	void set(const Vector3f& xyz, float w);
	//please set angle by using radian
	void setRotation(const Vector3f& axis, float angle);
	const Quaternion slerp(const Quaternion& q0, const Quaternion& q1, float t);
	const Quaternion& operator *=(const Quaternion& q);
	const Quaternion operator *(const Quaternion& q);

	const Vector3f calcRotation(const Vector3f&)const;

};

