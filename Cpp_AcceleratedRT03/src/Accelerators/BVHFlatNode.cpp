#include "BVHFlatNode.h"

BVHFlatNode::BVHFlatNode()
	: boundingBox(Vector3(), Vector3())
{
	this->startIndex = 0;
	this->numPrimitives = 0;
	this->rightOffset = 0;
}

BVHFlatNode::BVHFlatNode(const BoundingBox &boundingBox, int startIndex, int numPrimitives,
	int rightOffset)
	: boundingBox(boundingBox)
{
	this->startIndex = startIndex;
	this->numPrimitives = numPrimitives;
	this->rightOffset = rightOffset;
}

const BoundingBox &BVHFlatNode::getBoundingBox() const
{
	return this->boundingBox;
}

int BVHFlatNode::getStartIndex() const
{
	return this->startIndex;
}

int BVHFlatNode::getNumPrimitives() const
{
	return this->numPrimitives;
}

int BVHFlatNode::getRightOffset() const
{
	return this->rightOffset;
}

void BVHFlatNode::setBoundingBox(const BoundingBox &boundingBox)
{
	this->boundingBox = boundingBox;
}

void BVHFlatNode::setStartIndex(int startIndex)
{
	this->startIndex = startIndex;
}

void BVHFlatNode::setNumPrimitives(int numPrimitives)
{
	this->numPrimitives = numPrimitives;
}

void BVHFlatNode::setRightOffset(int rightOffset)
{
	this->rightOffset = rightOffset;
}