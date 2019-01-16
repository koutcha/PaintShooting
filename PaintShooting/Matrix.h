#pragma once

#include<array>
#include <algorithm>
#include <GL\glew.h>
#include "Quaternion.h"
#include "Vector3.h"

//transformation matrix
class Matrix
{
public:
	Matrix() {};
	Matrix(const GLfloat *a)
	{
		for (int i = 0; i < 16; ++i)
		{
			matrix[i] = a[i];
		}
	}
	//return the array of transformation matrix(raw array)
	const GLfloat* data()const
	{
		return matrix.data();
	}
	//set Identity Matrix
	void loadIdentity();
	//create Identity Matrix
	static Matrix Identity();
	//create translation matrix  to move by (x,y,z)
	static Matrix translate(GLfloat x, GLfloat y, GLfloat z);
	static Matrix translate(const Vector3f&);
	//create scaling matrix to scale by (x,y,z)
	static Matrix scale(GLfloat x, GLfloat y, GLfloat z);
	static Matrix scale(const Vector3f&);
	//create rotattion matrix by a-angle on (x,y,z) axisis
	static Matrix rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z);
	static Matrix rotate(const Quaternion& q);
	//create rotatin matrix by a-angle on X axisis
	static Matrix rotateX(GLfloat a);
	//create rotatin matrix by a-angle on Y axisis
	static Matrix rotateY(GLfloat a);
	//create rotatin matrix by a-angle on Z axisis
	static Matrix rotateZ(GLfloat a);
	//create Shear Matrix
	static Matrix shereXY(GLfloat x, GLfloat y, GLfloat z);
	static Matrix shereYX(GLfloat x, GLfloat y, GLfloat z);
	static Matrix shereYZ(GLfloat x, GLfloat y, GLfloat z);
	static Matrix shereZY(GLfloat x, GLfloat y, GLfloat z);
	static Matrix shereZX(GLfloat x, GLfloat y, GLfloat z);
	static Matrix shereXZ(GLfloat x, GLfloat y, GLfloat z);
	//create ViewTransformation Matrix
	static Matrix lookat(
		GLfloat ex, GLfloat ey, GLfloat ez,
		GLfloat gx, GLfloat gy, GLfloat gz,
		GLfloat ux, GLfloat uy, GLfloat);
	//create orthogonal matrix
	static Matrix orthogonal(
		GLfloat left, GLfloat right,
		GLfloat bottom, GLfloat top,
		GLfloat zNear, GLfloat zFar);
	//frustm
	static Matrix frustum(
		GLfloat left, GLfloat right,
		GLfloat bottom, GLfloat top,
		GLfloat zNear, GLfloat zFar);
	//perspectiv
	static Matrix perspective(
		GLfloat fovy, GLfloat aspect,
		GLfloat zNear, GLfloat zFar
	);
	//create TexTransformMatrix for change Tex coord
	static Matrix texture();
	//get transpose of the cofactor matrix that use for transforming normal 
	void getNormalMatrix(GLfloat *out)const;


	const Matrix operator*(const Matrix &m)const;
	Matrix& operator *=(const Matrix&m);

	const Vector3f operator*(const Vector3f&)const;

private:
	//factor in ransformation matrix
	std::array<GLfloat, 16> matrix;

};