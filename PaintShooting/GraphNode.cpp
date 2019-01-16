#include "GraphNode.h"


GraphNode::GraphNode(const Vector3f & position, const std::vector<unsigned int>& adjacentList):
	position(position),
	adjacent(adjacentList),
	isValid(true)
{

}

GraphNode::GraphNode(const Vector3f & position, std::vector<unsigned int>&& adjacentList):
	position(position),
	adjacent(adjacentList),
	isValid(true)
{

}

GraphNode::~GraphNode()
{

}

const std::vector<unsigned int>& GraphNode::getAdjacentList() const
{
	return adjacent;
}

const Vector3f GraphNode::getPosition() const
{
	return position;
}

bool GraphNode::getIsValid() const
{
	return isValid;
}
