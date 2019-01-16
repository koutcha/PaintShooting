#pragma once
#include "Camera.h"
class Matrix;
class OrthoCamera :
	public Camera
{
public:
	OrthoCamera();
	OrthoCamera(const Vector3f & position, const Vector3f & up, const Vector3f & gaze, 
		float left,float right,float bottom,float top,float near,float far);
	~OrthoCamera();
	void setXRange(float left, float right);
	void setYRange(float bottom, float top);
	
	void setCamera(GLuint viewLoc, GLuint projectionLoc) const;

private:
	float left;
	float right;
	float bottom;
	float top;
};

