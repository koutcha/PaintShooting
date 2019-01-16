#pragma once
#include <memory>
#include "Vector3.h"
class Shape;
class StageObstacle
{
public:
	StageObstacle();
	StageObstacle(const Vector3f& position,const Vector3f& scale,const std::shared_ptr<Shape>& shape);
	~StageObstacle();
	const Vector3f getPosition()const;
	const Vector3f getScale()const;
	const std::shared_ptr<Shape>& getShape()const;
	void setPosition(const Vector3f& value);
	void setScale(const Vector3f& value);
	void setShape(const std::shared_ptr<Shape>&);

	bool checkCollision(const Vector3f& position, float radius)const;
private:
	std::shared_ptr<Shape> shape;
	Vector3f position;
	Vector3f scale;
};

