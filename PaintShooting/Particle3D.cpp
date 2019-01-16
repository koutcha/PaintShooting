#include "Particle3D.h"
#include "ShapePrimitive.h"
#include "CreatePrimitiveShapes.h"
#include <iostream>


const std::shared_ptr<Particle3D> Particle3D::createSpherePartecle(int maxParticle)
{
	std::vector<Vertex> vertex;
	std::vector<GLuint> index;

	CreatePrimitiveShapes::createSphere(vertex, index, 5, 5);

	return std::make_shared<Particle3D>(maxParticle,3, vertex.size(), vertex.data(), index.size(), index.data());
}

Particle3D::Particle3D(int maxParticles, GLint size, GLsizei vertexcount, const Vertex * vertex, GLsizei indexcount, const GLuint * index) :
	shapeData(std::make_unique<ShapePrimitive>(size,vertexcount,vertex,indexcount,index)),
	indexCount(indexcount),
	transformData(maxParticles)
{
	shapeData->bind();
	glGenBuffers(1,&dataBuffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles *  sizeof(ParticleData), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
	glEnableVertexAttribArray(4);

	//ModelData‚ÌVertexPositon
	glVertexAttribDivisor(0, 0);
	//normal
	glVertexAttribDivisor(1, 0);
	//uv
	glVertexAttribDivisor(2, 0);
	//tangent
	glVertexAttribDivisor(3, 0);

	//ƒ‚ƒfƒ‹•ÏŠ·—p‚Ìpositon,scaleData
	glVertexAttribDivisor(4, 1);

	
	shapeData->unbind();
	
};



Particle3D::~Particle3D()
{
	shapeData->bind();
	glDeleteBuffers(1, &dataBuffer);
	shapeData->unbind();
}

void Particle3D::setData(unsigned int index, const ParticleData &data)
{
	if (index > transformData.size() - 1)
	{
		std::cout << "ERROR::PARTICLE3D::SETDATA  index out of bound (index = " << index <<")"<<std::endl;
		return;
	}
	transformData[index] = data;
}

void Particle3D::draw()const
{

	shapeData->bind();
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, transformData.size() * sizeof(ParticleData), transformData.data());;
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, transformData.size());
	shapeData->unbind();

}

void Particle3D::draw(unsigned int n)const
{
	if (n > transformData.size() - 1)
	{

		std::cout << "ERROR::PARTICLE3D::DRAW draw number out of bound" << std::endl;
		n = transformData.size();
	}

	shapeData->bind();
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, n* sizeof(ParticleData), transformData.data());;
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, n);
	shapeData->unbind();

}
