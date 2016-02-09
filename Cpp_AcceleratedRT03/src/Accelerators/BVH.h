#ifndef BVH_H
#define BVH_H

#include <memory>
#include <vector>

#include "Accelerator.h"
#include "../Utilities/Utility.h"

// Uses code from the "Fast-BVH" ray tracer by Brandon Pelfrey.

class BVH : public Accelerator
{
private:
	std::vector<const class Shape**> shapePtrs;
	std::vector<class BVHFlatNode> flatTree;
	int numNodes;
	int numLeaves;
	int leafCapacity;

	static const int DEFAULT_LEAF_CAPACITY = 4;
	static const int MAX_BVH_BUILD_TO_DO = 128;
	static const int MAX_BVH_TRAVERSAL_TO_DO = 128;
	static const int LEAF_NODE_RIGHT_OFFSET = 0;
	static const int UNTOUCHED = 0xFFFFFFFF;
	static const int TOUCHED_TWICE = 0xFFFFFFFD;
	static const int ROOT_START_INDEX = 0;
	static const int ROOT_PARENT_INDEX = 0xFFFFFFFC;
public:
	BVH(const std::vector<class Shape*> &shapes);

	virtual class Intersection nearestHit(const class Ray &ray) const override;
};

#endif