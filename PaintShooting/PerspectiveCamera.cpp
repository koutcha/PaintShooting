#include "PerspectiveCamera.h"
#include "Matrix.h"





PerspectiveCamera::PerspectiveCamera(float fovy, float aspect):
	Camera(),
	fovy(fovy),
	aspect(aspect)
{
}

PerspectiveCamera::PerspectiveCamera(const Vector3f & position, const Vector3f & up, const Vector3f & gaze, float near, float far, float fovy, float aspect):
	Camera(position,up,gaze,near,far),
	fovy(fovy),
	aspect(aspect)
{

}



void PerspectiveCamera::setCamera(GLuint viewLoc, GLuint projectionLoc) const
{
	const Matrix projection(Matrix::perspective(fovy, aspect,getNear(),getFar()));
	const Matrix view(Matrix::lookat(getPosition().x,getPosition().y,getPosition().z,getGaze().x,getGaze().y, getGaze().z,getUp().x, getUp().y,getUp().z));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::setAspect(float aspect)
{
	this->aspect = aspect;
}

void PerspectiveCamera::setFovy(float fovy)
{
	this->fovy = fovy;
}

Matrix PerspectiveCamera::getPVMatrix() const
{
	const Matrix projection(Matrix::perspective(fovy, aspect, getNear(), getFar()));
	const Matrix view(Matrix::lookat(getPosition().x, getPosition().y, getPosition().z, getGaze().x, getGaze().y, getGaze().z, getUp().x, getUp().y, getUp().z));
	return std::move(Matrix(projection*view));
}
