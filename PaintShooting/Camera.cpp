#include "Camera.h"



Camera::Camera():
	position(0,0,1),
	up(0,1,0),
	gaze(0,0,0),
	nearClip(1.0f),
	farClip(100.f)
{
}

Camera::Camera(const Vector3f & position, const Vector3f & up, const Vector3f & gaze, float near, float far):
	position(position),
	up(up),
	gaze(gaze),
	nearClip(near),
	farClip(far)
{
}


Camera::~Camera()
{
}


void Camera::setViewPos(GLuint viewPosLoc) const
{
	glUniform3fv(viewPosLoc, 1,position.getArray().data());
}

void Camera::setPosition(const Vector3f &p)
{
	position = p;
}

void Camera::setUp(const Vector3f &u)
{
	up = u;
}

void Camera::setGaze(const Vector3f &g)
{
	gaze = g;
}

void Camera::setFar(float far)
{
	farClip = far;
}

void Camera::setNear(float near)
{
	nearClip = near;
}

const Vector3f  Camera::getPosition() const
{
	return position;
}

const Vector3f  Camera::getUp() const
{
	return up;
}

const Vector3f  Camera::getGaze() const
{
	return gaze;
}

float Camera::getFar() const
{
	return farClip;
}

float Camera::getNear() const
{
	return nearClip;
}



