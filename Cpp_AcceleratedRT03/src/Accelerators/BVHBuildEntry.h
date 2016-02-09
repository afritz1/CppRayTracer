#ifndef BVH_BUILD_ENTRY_H
#define BVH_BUILD_ENTRY_H

#include "../Utilities/Utility.h"

class BVHBuildEntry
{
private:
	int startIndex;
	int endIndex;
	int parentIndex;
public:
	BVHBuildEntry();
	BVHBuildEntry(int startIndex, int endIndex, int parentIndex);

	int getStartIndex() const;
	int getEndIndex() const;
	int getParentIndex() const;
};

#endif