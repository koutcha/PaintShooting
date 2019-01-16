#pragma once
#include <memory>

#include "ShapePrimitive.h"

class Shape
{

public:
	Shape(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = nullptr);
	virtual ~Shape();
	void draw()const;
protected:
	const GLsizei vertexcount;
	virtual void execute()const;
private:
	std::unique_ptr<const ShapePrimitive> object;
};