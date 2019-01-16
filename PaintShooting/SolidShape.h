#pragma once

#include "Shape.h"
class SolidShape:
	public Shape
{
public:
	SolidShape(GLint size,GLsizei vertexcount,const Vertex *vertex);
	virtual ~SolidShape();
protected:
	virtual void execute() const override;
};

