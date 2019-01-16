#include "ShapeIndex.h"



ShapeIndex::ShapeIndex(GLint size, GLsizei vertexcount, const Vertex * vertex, GLsizei indexcount, const GLuint * index):
	 Shape(size, vertexcount, vertex, indexcount, index)
	,indexcount(indexcount)
{

}

ShapeIndex::~ShapeIndex()
{
}

void ShapeIndex::execute() const
{
	glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, 0);
}
