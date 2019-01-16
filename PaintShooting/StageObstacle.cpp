#include "StageObstacle.h"

#include "MathAndPhysic.h"

StageObstacle::StageObstacle():
	position(0.0,0.0,0.0),
	scale(0.0,0.0,0.0)
{

}

StageObstacle::StageObstacle(const Vector3f & position, const Vector3f & scale,const std::shared_ptr<Shape>& shape):
	position(position),
	scale(scale),
	shape(shape)
{


}


StageObstacle::~StageObstacle()
{
}

const Vector3f StageObstacle::getPosition() const
{
	return position;
}

const Vector3f StageObstacle::getScale() const
{
	return scale;
}

const std::shared_ptr<Shape>& StageObstacle::getShape() const
{
	return shape;
}



void StageObstacle::setPosition(const Vector3f & value)
{
	position = value;
}

void StageObstacle::setScale(const Vector3f & value)
{
	scale = value;
}

void StageObstacle::setShape(const std::shared_ptr<Shape>& shape)
{
	this->shape = shape;
}

bool StageObstacle::checkCollision(const Vector3f & position, float radius)const
{

	return 	MyMath::SquaringlengthAABBToPoint(this->position, scale, position) < radius*radius;
}

