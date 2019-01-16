#define _USE_MATH_DEFINES 
#include "Matrix.h"
#include "Vector3.h"
#include<cmath>
void Matrix::loadIdentity()
{
	std::fill(matrix.begin(),matrix.end(), 0.0f);
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

Matrix Matrix::Identity()
{
	Matrix m;
	m.loadIdentity();
	return m;
}

Matrix Matrix::translate(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[12] = x;
	m.matrix[13] = y;
	m.matrix[14] = z;
	return m;
}

Matrix Matrix::translate(const Vector3f &v)
{
	return translate(v.x, v.y, v.z);
}

Matrix Matrix::scale(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[0] = x;
	m.matrix[5] = y;
	m.matrix[10] = z;
	return m;
}

Matrix Matrix::scale(const Vector3f &v)
{
	return scale(v.x, v.y, v.z);
}

Matrix Matrix::rotate(GLfloat a,GLfloat x, GLfloat y, GLfloat z)
{
	Matrix t;
	const GLfloat d(sqrt(x * x + y * y + z * z));
	if (d > 0.0f)
	{
		const GLfloat l(x / d), m(y / d), n(z / d);
		const GLfloat l2(l * l), m2(m * m), n2(n * n);
		const GLfloat lm(l * m), mn(m * n), nl(n * l);
		const GLfloat c(cos(a)), c1(1.0f - c), s(sin(a));
		t.loadIdentity();
		t.matrix[0] = (1.0f - l2) * c + l2;
		t.matrix[1] = lm * c1 + n * s;
		t.matrix[2] = nl * c1 - m * s;
		t.matrix[4] = lm * c1 - n * s;
		t.matrix[5] = (1.0f - m2) * c + m2;
		t.matrix[6] = mn * c1 + l * s;
		t.matrix[8] = nl * c1 + m * s;
		t.matrix[9] = mn * c1 - l * s;
		t.matrix[10] = (1.0f - n2) * c + n2;
	}
	return t;
}

Matrix Matrix::rotate(const Quaternion & q)
{
	Matrix t;
	t.loadIdentity();
	float q1, q2, q3, q0;
	q1 = q.v.x;
	q2 = q.v.y;
	q3 = q.v.z;
	q0 = q.w;
	float q1_2, q2_2, q3_2, q0_2;
	q1_2 = q1 * q1;
	q2_2 = q2 * q2;
	q3_2 = q3 * q3;
	q0_2 = q0 * q0;

	t.matrix[0] = q0_2 + q1_2 - q2_2 - q3_2;
	t.matrix[4] = 2.0f*(q1*q2 - q0 * q3);
	t.matrix[8] = 2.0f*(q1*q3 + q0 * q2);

	t.matrix[1] = 2.0f*(q1*q2 + q0 * q3);
	t.matrix[5] = q0_2 - q1_2 + q2_2 - q3_2;
	t.matrix [9] = 2.0f*(q2*q3 - q0 * q1);

	t.matrix[2]= 2.0f*(q1*q3 - q0 * q2);
	t.matrix[6] = 2.0f*(q2*q3 + q0 * q1);
	t.matrix[10] = q0_2 - q1_2 - q2_2 + q3_2;

	t.matrix[3] = t.matrix[7] = t.matrix[11] = 0;
	t.matrix[15] = 1.0f;

	//ÉIÉCÉâÅ[äpÇÃåvéZèàóù
	//float yAngle, xAngle, zAngle;

	//if (t.matrix[6] == 1.0) {
	//	xAngle = M_PI/2.0;
	//	yAngle = 0;
	//	zAngle = atan2(t.matrix[1], t.matrix[0]);
	//}
	//else if (t.matrix[6] == -1.0) {
	//	xAngle = -M_PI / 2.0;
	//	yAngle = 0;
	//	zAngle = atan2(t.matrix[1],t.matrix[0]);
	//}
	//else {
	//	xAngle = asin(-t.matrix[6]);
	//	yAngle = atan2(t.matrix[2], t.matrix[10]);
	//	zAngle = atan2(t.matrix[4], t.matrix[5]);
	//}

	//Vector3 angle(xAngle, yAngle, zAngle);
	//angle.debugWrite("angle");
	return t;
}

Matrix Matrix::rotateX(GLfloat a)
{
	Matrix m;
	m.loadIdentity();
	const GLfloat c(cos(a));
	const GLfloat s(sin(a));
	m.matrix[5] = c;
	m.matrix[6] = s;
	m.matrix[9] = -s;
	m.matrix[10] = c;
	return m;
}

Matrix Matrix::rotateY(GLfloat a)
{
	Matrix m;
	m.loadIdentity();
	const GLfloat c(cos(a));
	const GLfloat s(sin(a));
	m.matrix[0] = c;
	m.matrix[2] = -s;
	m.matrix[8] = s;
	m.matrix[10] = c;

	return m;
}

Matrix Matrix::rotateZ(GLfloat a)
{
	Matrix m;
	m.loadIdentity();
	const GLfloat c(cos(a));
	const GLfloat s(sin(a));
	m.matrix[0] = c;
	m.matrix[1] = s;
	m.matrix[4] = -s;
	m.matrix[5] = c;
	return m;
}

Matrix Matrix::shereXY(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[4];

	return m;
}

Matrix Matrix::shereYX(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[1];

	return m;
}
Matrix Matrix::shereYZ(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[9];

	return m;
}

Matrix Matrix::shereZY(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[6];

	return m;
}

Matrix Matrix::shereZX(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[2];

	return m;
}

Matrix Matrix::shereXZ(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix m;
	m.loadIdentity();
	m.matrix[8];

	return m;
}

Matrix Matrix::lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat gx, GLfloat gy, GLfloat gz, GLfloat ux, GLfloat uy, GLfloat uz)
{
	//e : camera position
	//g : camera target position
	//u : up direction of the camera

	//Parallel displacement matrix for transforming e to origin;
	const Matrix tv(translate(-ex, -ey, -ez));

	//t axsis= e - g(eye vector
	const GLfloat tx(ex - gx);
	const GLfloat ty(ey - gy);
	const GLfloat tz(ez - gz);
	//r axisis = u cross t
	const GLfloat rx(uy * tz - uz * ty);
	const GLfloat ry(uz * tx - ux * tz);
	const GLfloat rz(ux * ty - uy * tx);
	//s axsis = t cross r
	const GLfloat sx(ty * rz - tz * ry);
	const GLfloat sy(tz * rx - tx * rz);
	const GLfloat sz(tx * ry - ty * rx);

	//chcek s's length(if t direction == s direction,s's length is 0)
	const GLfloat s2(sx*sx + sy * sy + sz * sz);
	if (s2 == 0.0f) return tv;
	//rotation matrix
	Matrix rv;
	rv.loadIdentity();
	//normalize r-axsis and set it into array
	const GLfloat r(sqrt(rx*rx + ry * ry + rz * rz));
	rv.matrix[0] = rx / r;
	rv.matrix[4] = ry / r;
	rv.matrix[8] = rz / r;

	//same operation for s
	const GLfloat s(sqrt(s2));
	rv.matrix[1] = sx / s;
	rv.matrix[5] = sy / s;
	rv.matrix[9] = sz / s;

	//same operation for t
	const GLfloat t(sqrt(tx*tx + ty * ty + tz * tz));
	rv.matrix[2] = tx / t;
	rv.matrix[6] = ty / t;
	rv.matrix[10] = tz / t;

	return rv*tv;
}

Matrix Matrix::orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	Matrix m;
	const GLfloat dx(right - left);
	const GLfloat dy(top - bottom);
	const GLfloat dz(zFar - zNear);

	if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
	{
		m.loadIdentity();
		m.matrix[0] = 2.0f / dx;
		m.matrix[5] = 2.0f / dy;
		m.matrix[10] = -2.0f / dz;
		m.matrix[12] = -(right + left) / dx;
		m.matrix[13] = -(top + bottom) / dy;
		m.matrix[14] = -(zFar + zNear) / dz;
	}
	return m;
}

Matrix Matrix::frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	Matrix m;
	const GLfloat dx(right - left);
	const GLfloat dy(top - bottom);
	const GLfloat dz(zFar - zNear);
	if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
	{
		m.loadIdentity();
		m.matrix[0] = 2.0f * zNear / dx;
		m.matrix[5] = 2.0f * zNear / dy;
		m.matrix[8] = (right + left) / dx;
		m.matrix[9] = (top + bottom) / dy;
		m.matrix[10] = -(zFar + zNear) / dz;
		m.matrix[11] = -1.0f;
		m.matrix[14] = -2.0f * zFar * zNear / dz;
		m.matrix[15] = 0.0f;
	}
	return m;

}
//fleild of view(0~4/pi) aspectRatio, near
Matrix Matrix::perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	Matrix m;
	const GLfloat dz(zFar - zNear);

	if (dz != 0.0f)
	{
		m.loadIdentity();
		m.matrix[5] = 1.0f / tan(fovy*0.5f);
		m.matrix[0] = m.matrix[5] / aspect;
		m.matrix[10] = -(zFar + zNear) / dz;
		m.matrix[11] = -1.0f;
		m.matrix[14] = -2.0f*zFar*zNear / dz;
		m.matrix[15] = -0.0f;
	}
	return m;
}

Matrix Matrix::texture()
{
	Matrix m;
	m.Identity();
	m.matrix[0] = 0.5; m.matrix[1] = 0.0; m.matrix[2] = 0.0; m.matrix[3] = 0.0;
	m.matrix[4] = 0.0; m.matrix[5] = 0.5; m.matrix[6] = 0.0; m.matrix[7] = 0.0;
	m.matrix[8] = 0.0; m.matrix[9] = 0.0; m.matrix[10] = 1.0; m.matrix[11] = 0.0;
	m.matrix[12] = 0.5; m.matrix[13] = 0.5; m.matrix[14] = 0.0; m.matrix[15] = 1.0;
	return m;
}

void Matrix::getNormalMatrix(GLfloat * m) const
{
	m[0] = matrix[5] * matrix[10] - matrix[6] * matrix[9];
	m[1] = matrix[6] * matrix[8] - matrix[4] * matrix[10];
	m[2] = matrix[4] * matrix[9] - matrix[5] * matrix[8];
	m[3] = matrix[9] * matrix[2] - matrix[10] * matrix[1];
	m[4] = matrix[10] * matrix[0] - matrix[8] * matrix[2];
	m[5] = matrix[8] * matrix[1] - matrix[9] * matrix[0];
	m[6] = matrix[1] * matrix[6] - matrix[2] * matrix[5];
	m[7] = matrix[2] * matrix[4] - matrix[0] * matrix[6];
	m[8] = matrix[0] * matrix[5] - matrix[1] * matrix[4];
}

const Matrix Matrix::operator*(const Matrix & m) const
{
	Matrix t;
	for (int i = 0; i < 16; ++i)
	{
		const int j(i & 3), k(i & ~3);
		t.matrix[i] =
			matrix[0 + j] * m.matrix[k + 0] +
			matrix[4 + j] * m.matrix[k + 1] +
			matrix[8 + j] * m.matrix[k + 2] +
			matrix[12 + j] * m.matrix[k + 3];
	}
	return t;
}

Matrix & Matrix::operator*=(const Matrix & m)
{
	*this = *this*m;

	return *this;
}

const Vector3f Matrix::operator*(const Vector3f& v) const
{
	Vector3f out;
	out.x = matrix[0] * v.x + matrix[4] * v.y + matrix[8] * v.z + matrix[12]*1.0f;
	out.y = matrix[1] * v.x + matrix[5] * v.y + matrix[9] * v.z + matrix[13] * 1.0f;
	out.z = matrix[2] * v.x + matrix[6] * v.y + matrix[10] * v.z + matrix[14] * 1.0f;
	float w = matrix[3] * v.x + matrix[7] * v.y + matrix[11] * v.z + matrix[15] * 1.0f;
	out /= w;
	return out;
}
