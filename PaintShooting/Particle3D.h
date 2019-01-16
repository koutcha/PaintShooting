#pragma once
#include <GL/glew.h>
#include <memory>
#include <vector>
#include "Vector3.h"
class ShapePrimitive;
struct Vertex;
struct ParticleData
{
	Vector3f position;
	float size;
};
class Particle3D
{
public:
	static const std::shared_ptr<Particle3D> createSpherePartecle(int maxParticle);

	Particle3D(int maxParticle, GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = nullptr);

	~Particle3D();
	void setData(unsigned int index,const ParticleData&);
	void draw()const;
	void draw(unsigned int n)const;
private:
	
	std::unique_ptr<ShapePrimitive> shapeData;
	std::vector<ParticleData> transformData;
	unsigned int dataBuffer;
	unsigned int indexCount;
};

