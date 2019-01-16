#include "Shape.h"
#include<iostream>
Shape::Shape(GLint size, GLsizei vertexcount, const Vertex * vertex,GLsizei indexcount,const GLuint *index):
	object(new ShapePrimitive(size,vertexcount,vertex,indexcount,index)),
	vertexcount(vertexcount)
{

}

Shape::~Shape()
{
}

void Shape::draw()const
{
	object->bind();
	execute();
	object->unbind();
}

void Shape::execute()const
{
	glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
}
