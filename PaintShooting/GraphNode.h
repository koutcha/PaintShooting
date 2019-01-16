#pragma once
#include <vector>
#include "Vector3.h"
class GraphNode
{
public:
	GraphNode(const Vector3f& position,const std::vector<unsigned int>& adjacentList);
	GraphNode(const Vector3f& position, std::vector<unsigned int>&& adjacentList);
	~GraphNode();

	const std::vector<unsigned int>& getAdjacentList()const;
	const Vector3f getPosition()const;
	bool getIsValid()const;
private:
	bool isValid;
	std::vector<unsigned int> adjacent;
	Vector3f position;
};

