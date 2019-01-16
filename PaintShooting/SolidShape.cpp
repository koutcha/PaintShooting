#include "SolidShape.h"


SolidShape::SolidShape(GLint size, GLsizei vertexcount, const Vertex * vertex):
	Shape(size,vertexcount,vertex)
{
}

SolidShape::~SolidShape()
{
}

void SolidShape::execute() const
{
	glDrawArrays(GL_TRIANGLES, 0, vertexcount);
}
