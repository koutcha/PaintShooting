#include "ShapePrimitive.h"

void ShapePrimitive::bind() const
{

	glBindVertexArray(vao);
}

void ShapePrimitive::unbind() const
{
	glBindVertexArray(0);
}
