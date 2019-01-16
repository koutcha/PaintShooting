#pragma once
template<typename T>
class Vector2
{
public:
	T x;
	T y;

	Vector2();
	Vector2(const T& x,const T& y);
	~Vector2();
	
	Vector2<T>& operator*=(const T& f);
	Vector2<T>& operator +=(const Vector2<T>&);
	Vector2<T>& operator -=(const Vector2<T>&);
	Vector2<T>& operator /=(const T& f);


	const Vector2<T> operator+(const Vector2<T>&)const;
	const Vector2<T> operator-(const Vector2<T>&)const;
	
	void set(const T& x, const T& y);
};


template<typename T>
Vector2<T>::Vector2()
{
}

template<typename T>
Vector2<T>::Vector2(const T & x, const T & y) :
	x(x),
	y(y)
{

}

template<typename T>
Vector2<T>::~Vector2()
{
}




template<typename T>
Vector2<T>& Vector2<T>::operator*=(const T & f)
{
	x *= f;
	x *= f;
	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T> &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T> &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator/=(const T & f)
{
	x *= f;
	x *= f;
	return *this;
}

template<typename T>
const Vector2<T> Vector2<T>::operator+(const Vector2<T> &v) const
{
	Vector2 result(x + v.x, y + v.y);
	return std::move(result);
}
template<typename T>
const Vector2<T> Vector2<T>::operator-(const Vector2<T> &v) const
{
	Vector2 result(x - v.x, y - v.y);
	return std::move(result);
}

template<typename T>
void Vector2<T>::set(const T & x, const T & y)
{
	this->x = x;
	this->y = y;
}



typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;