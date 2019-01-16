#pragma once
#include <vector>
#include <GL/glew.h>

struct Vertex;
namespace CreatePrimitiveShapes
{
	void createSphere(std::vector<Vertex>& vertexBuffer,std::vector<GLuint>& indexBuffer,int slice,int stacks);
	void createBox(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& indexBuffer);
}