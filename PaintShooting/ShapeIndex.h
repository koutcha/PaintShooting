#pragma once

#include "Shape.h"
class ShapeIndex:
	public Shape
{
public:
	ShapeIndex(GLint size,GLsizei veetexcount,const Vertex *vertex,GLsizei indexcount,const GLuint *index);
	virtual ~ShapeIndex();
protected:
	virtual void execute() const;
	const GLsizei indexcount;

};

