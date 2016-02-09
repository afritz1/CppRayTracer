#include "BVHTraversal.h"

BVHTraversal::BVHTraversal()
{
	this->index = 0;
	this->minT = 0;
}

BVHTraversal::BVHTraversal(int index, double minT)
{
	this->index = index;
	this->minT = minT;
}

int BVHTraversal::getIndex() const
{
	return this->index;
}

double BVHTraversal::getMinT() const
{
	return this->minT;
}