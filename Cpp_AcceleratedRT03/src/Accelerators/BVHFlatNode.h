#ifndef BVH_FLAT_NODE_H
#define BVH_FLAT_NODE_H

#include "BoundingBox.h"
#include "../Utilities/Utility.h"

class BVHFlatNode
{
private:
	BoundingBox boundingBox;
	int startIndex, numPrimitives, rightOffset;
public:
	BVHFlatNode();
	BVHFlatNode(const BoundingBox &boundingBox, int startIndex, int numPrimitives,
		int rightOffset);

	const BoundingBox &getBoundingBox() const;
	int getStartIndex() const;
	int getNumPrimitives() const;
	int getRightOffset() const;
	void setBoundingBox(const BoundingBox &boundingBox);
	void setStartIndex(int startIndex);
	void setNumPrimitives(int numPrimitives);
	void setRightOffset(int rightOffset);
};

#endif