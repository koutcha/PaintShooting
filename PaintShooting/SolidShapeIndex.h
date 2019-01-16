#pragma once

#include "ShapeIndex.h"
#include <memory>
class SolidShapeIndex:
	public ShapeIndex
{
public:
	SolidShapeIndex(GLint size,GLsizei vertexcount,const Vertex *vertex,
		GLsizei indexcount,const GLuint *index);
	virtual ~SolidShapeIndex();
	 
	 static std::shared_ptr<Shape>  createSphereShape(int slices,int stacks,float radius);
	 static std::shared_ptr<Shape> createBoxShape();
	 static std::shared_ptr<Shape> createBoxShape(float uvScaleX,float uvScaleY,float uvScaleZ);
	 static std::shared_ptr<Shape>	createPlaneShape(float width,float depth);
	 //get front +Z plane 
	 static std::shared_ptr<Shape> createVerticalPlane(float width, float height);
	 static std::shared_ptr<Shape> createTriangleShape();
private:
	void  execute() const override;
};

