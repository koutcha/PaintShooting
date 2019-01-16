#pragma once
#include <string>
#include<array>
#include<iostream>
namespace
{

}
template<typename T>
class Vector3{
public:

	T x;
	T y;
	T z;

	Vector3();
	Vector3(const T& x,const T& y,const T& z);
	Vector3(const std::array<T, 3>& data);

	Vector3<T>& operator*=(const T& f);
	Vector3<T>& operator +=(const Vector3<T>&);
	Vector3<T>& operator -=(const Vector3<T>&);
	Vector3<T>& operator /=(const T& f);
	


	const Vector3<T> operator+(const Vector3<T>&)const;
	const Vector3<T> operator-(const Vector3<T>&)const;
	const Vector3<T> operator*(const T&)const; 
	const Vector3<T> operator/(const T&)const;

	void set(const T& x,const T& y,const T& z);
	void setByIndex(int index,const T& value);
	const T getByIndex(int index) const;


	void normalize();

	const T dot(const Vector3<T>&)const;
	const Vector3<T> cross(const Vector3<T> & v) const;
	const T  length()const;
	const T squaredLength()const;
	const std::array<T, 3> getArray()const;
	const T * data() const;

	void debugWrite(const char* vectorName)const;
private:

	
};
template<typename T>
Vector3<T>::Vector3()
{

}
template<typename T>
Vector3<T>::Vector3(const T& x,const T& y,const T& z) :
	x(x), y(y), z(z)
{
}
template<typename T>
Vector3<T>::Vector3(const std::array<T, 3>& data)
{
	x = data[0];
	y = data[1];
	z = data[2];
}
template<typename T>
Vector3<T>& Vector3<T>::operator*=(const T& f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}
template<typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T> &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
template<typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T> &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
template<typename T>
Vector3<T> & Vector3<T>::operator/=(const T& f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}
template<typename T>
const Vector3<T> Vector3<T>::operator+(const Vector3<T> &v) const
{
	Vector3<T> result(*this);
	result += v;
	return result;
}
template<typename T>
const Vector3<T> Vector3<T>::operator-(const Vector3<T>&v) const
{

	Vector3<T> result(*this);
	result -= v;
	return result;
}

template<typename T>
inline const Vector3<T> Vector3<T>::operator*(const T &s) const
{
	Vector3<T>	result(*this);
	result *= s;
	return result;
}

template<typename T>
inline const Vector3<T> Vector3<T>::operator/(const T &s) const
{
	Vector3<T>	result(*this);
	result /= s;
	return result;
}


template<typename T>
void Vector3<T>::set(const T&  x, const T& y, const T&  z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
template<typename T>
void Vector3<T>::setByIndex(int index, const T&  value)
{
	switch (index) {
	case 0:
		x = value;
		break;
	case 1:
		y = value;
		break;
	case 2:
		z = value;
		break;
	default:
		break;
	}
}

template<typename T>
inline const T Vector3<T>::getByIndex(int index) const
{
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		return 0;
	}
}

template<typename T>
void Vector3<T>::normalize()
{
	T length = this->length();
	if (length > 0) {
		*this /= length;
	}
}


template<typename T>
const T Vector3<T>::dot(const Vector3<T> &v) const
{
	return x * v.x + y * v.y + z * v.z;
}
template<typename T>
const Vector3<T> Vector3<T>::cross(const Vector3<T>&v)const
{
	Vector3<T> temp;
	temp.x = y * v.z - v.y*z;
	temp.y = z * v.x - v.z*x;
	temp.z = x * v.y - v.x*y;
	return std::move(temp);
}
template<typename T>
const T Vector3<T>::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

template<typename T>
inline const T Vector3<T>::squaredLength() const
{
	return x * x + y * y + z * z;
}

template<typename T>
inline const std::array<T, 3> Vector3<T>::getArray() const
{
	return std::array<T, 3>({x,y,z});
}

template<typename T>
inline const T * Vector3<T>::data() const
{
	return &x;
}


template<typename T>
void Vector3<T>::debugWrite(const char* vectorName) const
{
	std::cout << vectorName << "(" << x << "," << y << "," << z << ")" << std::endl;
}

using  Vector3f = Vector3<float>;
using  Vector3d = Vector3<double>;