#pragma once
#include <array>
#include <GL/glew.h>
//vertex Attribute
struct Vertex
{

	std::array<GLfloat, 3> position;
	std::array<GLfloat, 3> normal;
	std::array<GLfloat, 2> uv;
	std::array<GLfloat, 3> tangent;

};