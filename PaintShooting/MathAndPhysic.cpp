
#include "MathAndPhysic.h"
#include<array>
#include <iostream>
#include "Vector2.h"
float MyMath::randamFloat(float max, float min, int split)
{
	return (max - min)*(static_cast<float>(rand() % split) / static_cast<float>(split)) + min;
}
//距離の二乗を返す
float MyMath::SquaringlengthAABBToPoint(const Vector3f & boxCenter, const Vector3f & boxSize, const Vector3f & point)
{
	Vector3f max(boxCenter.x+boxSize.x,boxCenter.y + boxSize.y, boxCenter.z + boxSize.z);
	Vector3f min(boxCenter.x - boxSize.x, boxCenter.y - boxSize.y, boxCenter.z - boxSize.z);
	float length = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (point.getByIndex(i) < min.getByIndex(i))
		{
			length += (point.getByIndex(i) - min.getByIndex(i))* (point.getByIndex(i) - min.getByIndex(i));
		}
		else if(point.getByIndex(i) > max.getByIndex(i))
		{
			length += (point.getByIndex(i) - max.getByIndex(i))* (point.getByIndex(i) - max.getByIndex(i));
		}			

	}
	return length;
}
Vector3f MyMath::Vector3AABBToPoint(const Vector3f & boxCenter, const Vector3f & boxSize, const Vector3f & point)
{
	Vector3f v(0, 0, 0);
	Vector3f max(boxCenter.x + boxSize.x, boxCenter.y + boxSize.y, boxCenter.z + boxSize.z);
	Vector3f min(boxCenter.x - boxSize.x, boxCenter.y - boxSize.y, boxCenter.z - boxSize.z);
	float distance = 0;
	for (int i = 0; i < 3; ++i)
	{
		distance = 0;
		if (point.getByIndex(i) < min.getByIndex(i))
		{
			distance = point.getByIndex(i) - min.getByIndex(i);
		}
		else 	if (point.getByIndex(i) > max.getByIndex(i))
		{
			distance = point.getByIndex(i) - max.getByIndex(i);
		}
		

		v.setByIndex(i, distance);

	}
	
	return v;

}

Vector3f MyMath::calcReflex(const Vector3f& v1, const Vector3f& action, float m, float restitution)
{
	//作用線の標準化
	Vector3f n = action;
	n.normalize();
	//n.debugWrite("n");
	//速度の標準化
	Vector3f temp1 = v1;
	temp1.normalize();
	//内積からコサインを求める
	float cos1 = temp1.dot(n);
	//作用線を渡す
	temp1 = n;
	//作用線に速度の長さとコサインをかけて作用線上の速度を求める
	temp1 *= v1.length()*cos1;
	//
	float  coefficient = -(restitution + 1)*m;
	Vector3f ft = temp1 * coefficient;

	return ft;
}
Vector3f MyMath::calcImpulse(const Vector3f & v1, const Vector3f v2, const Vector3f & action, float m1, float m2, float restitution)
{
	//作用方向ベクトル
	Vector3f n(action);
	n.normalize();
	//n.debugWrite("n");

	Vector3f temp1(v1);
	temp1.normalize();
	Vector3f temp2(v2);
	temp2.normalize();

	//temp1.debugWrite("t1");
	//temp2.debugWrite("t2");

	float cos1 = temp1.dot(n);

	float cos2 = temp2.dot(n);

	//std::cout << "cos1,cos2(" << cos1 << "," << cos2 << ")"<<std::endl;

	temp1 = temp2 = n;
	temp1 *= v1.length()*cos1;
	temp2 *= v2.length()*cos2;

	Vector3f relative =  temp1 - temp2;

	float denominator = (m1 + m2) / (m1 * m2);
	float coefficient = (restitution + 1.0f)/denominator;
	relative *= -coefficient;
	//relative.debugWrite("relative");
	return relative;
}

Vector3f MyMath::calcTangent(const Vertex & v0,const Vertex & v1,const  Vertex & v2)
{
	Vector3f edge1(v1.position[0] - v0.position[0], v1.position[1] - v0.position[1], v1.position[2] - v0.position[2]);
	Vector3f edge2(v2.position[0] - v0.position[0], v2.position[1] - v0.position[1], v2.position[2] - v0.position[2]);

	Vector2f deltaUV1(v1.uv[0] - v0.uv[0], v1.uv[1] - v0.uv[1]);
	Vector2f deltaUV2(v2.uv[0] - v0.uv[0], v2.uv[1] - v0.uv[1]);

	float f = 1.0f / (deltaUV1.x*deltaUV2.y - deltaUV2.x*deltaUV1.y);
	Vector3f tangent(0, 0, 0);
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent.normalize();
	return tangent;
	
}

Vector3d MyMath::calcTangent(const std::array<double,11> v0, const std::array<double, 11> v1, const std::array<double, 11> & v2)
{
	Vector3d edge1(v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]);
	Vector3d edge2(v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]);

	Vector2d deltaUV1(v1[6] - v0[6], v1[7] - v0[7]);
	Vector2d deltaUV2(v2[6] - v0[6], v2[7] - v0[7]);

	double d = 1.0 / (deltaUV1.x*deltaUV2.y - deltaUV2.x*deltaUV1.y);
	Vector3d tangent(0, 0, 0);
	tangent.x = d * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = d * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = d * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent.normalize();
	return tangent;

}

Vector3f MyMath::calcNormal(const Vertex & v0, const Vertex & v1, const Vertex & v2)
{

	Vector3f v01(Vector3f(v1.position) - Vector3f(v0.position));
	Vector3f v02(Vector3f(v2.position) - Vector3f(v0.position));

	return v01.cross(v02);
}

int MyMath::sign(float f)
{
	if (f> 0) {
		return 1;
	}
	else if (f < 0) {
		return -1;
	}
	else
	{
		return 0;
	}
}

int MyMath::sign(double d)
{
	if (d > 0) {
		return 1;
	}
	else if (d < 0) {
		return -1;
	}
	else
	{
		return 0;
	}
}

int MyMath::sign(int i)
{
	if (i > 0) {
		return 1;
	}
	else if (i < 0) {
		return -1;
	}
	else
	{
		return 0;
	}
}
