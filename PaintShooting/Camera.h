#pragma once
#include"Vector3.h"
#include "GL\glew.h"
class Matrix;
class Camera
{
public:
	Camera();
	Camera(const Vector3f&position, const Vector3f& up, const Vector3f& gaze, float near, float far);
	virtual ~Camera();
	virtual void setCamera(GLuint viewLoc, GLuint projectionLoc) const = 0;
	virtual void setViewPos(GLuint viewPosLoc)const;
	void setPosition(const Vector3f&);
	void setUp(const Vector3f&);
	void setGaze(const Vector3f&);
	void setFar(float);
	void setNear(float);

	const Vector3f getPosition()const;
	const Vector3f getUp()const;
	const Vector3f getGaze()const;

	float getFar()const;
	float getNear()const;

	

private:
	Vector3f position;
	Vector3f up;
	Vector3f gaze;

	float farClip;
	float nearClip;
	
};

