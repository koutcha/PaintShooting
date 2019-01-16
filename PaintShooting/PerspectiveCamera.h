#pragma once
#include "Camera.h"
class PerspectiveCamera :
	public Camera
{
public:
	PerspectiveCamera(float fovy,float aspect);
	//up‚Æpositon‚Ægaze‚ÌŠÖŒW‚É‚æ‚Á‚ÄŠOÏ‚Æ“àÏ’×‚ê‚Ä•\¦‚ª‚¨‚©‚µ‚­‚È‚éê‡‚ª‚ ‚éi‚¨‚»‚ç‚­Šî’ê‚ÌŸ”‚ªŒ¸‚é
	PerspectiveCamera(const Vector3f& position, const Vector3f& up, const Vector3f& gaze, 
		float near, float far,
		float fovy, float aspect);

	~PerspectiveCamera();
	void setAspect(float aspect);
	void setFovy(float fovy);
	void setCamera(GLuint viewLoc, GLuint projectionLoc) const override;
	Matrix getPVMatrix()const;

private:
	float fovy;
	float aspect;
};

