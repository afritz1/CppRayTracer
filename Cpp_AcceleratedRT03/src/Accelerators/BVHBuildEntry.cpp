#include "BVHBuildEntry.h"

BVHBuildEntry::BVHBuildEntry()
{
	this->startIndex = 0;
	this->endIndex = 0;
	this->parentIndex = 0;
}

BVHBuildEntry::BVHBuildEntry(int startIndex, int endIndex, int parentIndex)
{
	this->startIndex = startIndex;
	this->endIndex = endIndex;
	this->parentIndex = parentIndex;
}

int BVHBuildEntry::getStartIndex() const
{
	return this->startIndex;
}

int BVHBuildEntry::getEndIndex() const
{
	return this->endIndex;
}

int BVHBuildEntry::getParentIndex() const
{
	return this->parentIndex;
}