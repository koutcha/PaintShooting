#define _USE_MATH_DEFINES
#include "Quaternion.h"
#include <cmath>




Quaternion::Quaternion()
{
}
Quaternion::Quaternion(const Vector3f& xyz,float w):
	v(xyz),w(w)
{
}

Quaternion::~Quaternion()
{
}

void Quaternion::setConjugate(const Quaternion & q)
{
	w = q.w;
	v = q.v;
	v *= -1.0;
}

void Quaternion::set(const Vector3f & xyz, float w)
{
	v = xyz;
	this->w = w;
}

void Quaternion::setRotation(const Vector3f & axis, float angle)
{
	v = axis;
	v.normalize();

	v.x *= sin(angle/2.0f);
	v.y *= sin(angle/2.0f);
	v.z *= sin(angle/2.0f);
	w = cos(angle/2.0f);
	//v.debugWrite("q");
}

const Quaternion Quaternion::slerp(const Quaternion & q0, const Quaternion & q1, float t)
{
	
	float x0 = q0.v.x,
		y0 = q0.v.y, 
		z0 = q0.v.z,
		w0 = q0.w;
	float x1 = q1.v.x,
		y1 = q1.v.y,
		z1 = q1.v.z,
		w1 = q1.w;

	float cosOmega = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1;

	if (cosOmega < 0)
	{
		x1 = -x1;
		y1 = -y1;
		z1 = -z1;
		w1 = -w1;
		cosOmega = -cosOmega;
	}

	float k0,k1;

	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;

		k0 = sin((1.0f - t)*omega)*oneOverSinOmega;
		k1 = sin(t*omega)*oneOverSinOmega;
	}
	Quaternion output;
	output.v.x = x0 * k0 + x1 * k1;
	output.v.y = y0 * k0 + y1 * k1;
	output.v.z=  z0 * k0 + z1 * k1;
	output.w = w0 * k0 + w1 * k1;

	return output;
}

const Quaternion& Quaternion::operator*=(const Quaternion & q)
{
	Vector3f cross, wd_v, w_vd;
	wd_v = v;
	w_vd = q.v;
	wd_v *= q.w;
	w_vd *= w;
	//cross.setCross(v, q.v);
	cross = v.cross(q.v);
	w = w * q.w - v.dot(q.v);
	v = w_vd;
	v += wd_v;
	v += cross;

	return *this;
}
const Quaternion Quaternion::operator*(const Quaternion & q)
{
	Quaternion result;
	result = *this;
	result *= q;
	return result;
}

const Vector3f Quaternion::calcRotation(const Vector3f &v) const
{
	Quaternion x(v, 0.0f);
	Quaternion conjugate;
	conjugate.setConjugate(*this);
	Quaternion result = *this;
	result *= x;
	result *= conjugate;

	return result.v;
}

