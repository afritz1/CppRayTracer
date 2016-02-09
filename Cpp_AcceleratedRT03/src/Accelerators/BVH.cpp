#include "BVH.h"
#include "BVHBuildEntry.h"
#include "BVHFlatNode.h"
#include "BVHTraversal.h"
#include "../Intersections/Intersection.h"
#include "../Rays/Ray.h"
#include "../Shapes/Shape.h"

BVH::BVH(const std::vector<Shape*> &shapes)
	: Accelerator()
{
	int shapeCount = static_cast<int>(shapes.size());
	this->shapePtrs = std::vector<const Shape**>(shapeCount);
	for (int i = 0; i < shapeCount; i++)
	{
		this->shapePtrs[i] = const_cast<const Shape**>(&shapes[i]);
	}

	this->flatTree = std::vector<BVHFlatNode>(shapePtrs.size() * 2);

	this->numNodes = 0;
	this->numLeaves = 0;
	this->leafCapacity = BVH::DEFAULT_LEAF_CAPACITY;

	std::vector<BVHBuildEntry> workArray =
		std::vector<BVHBuildEntry>(BVH::MAX_BVH_BUILD_TO_DO);

	// Put the root shape into the bounding volume hierarchy.
	workArray[0] = BVHBuildEntry(BVH::ROOT_START_INDEX, shapeCount, BVH::ROOT_PARENT_INDEX);

	// The build node index of the flat tree. Identical to the C "build_nodes_index".
	// Instead of using a pointer to the flat tree, just index the flat tree itself.
	int flatTreeIndex = 0;

	BVHFlatNode flatNode = BVHFlatNode();

	int stackIndex = 1;
	while (stackIndex > 0)
	{
		stackIndex--;
		const BVHBuildEntry &buildNode = workArray[stackIndex];

		this->numNodes++;

		flatNode.setStartIndex(buildNode.getStartIndex());
		flatNode.setNumPrimitives(buildNode.getEndIndex() - buildNode.getStartIndex());
		flatNode.setRightOffset(BVH::UNTOUCHED);

		// Calculate the bounding box for this flat node.
		BoundingBox nodeBox = (*this->shapePtrs[buildNode.getStartIndex()])->getBoundingBox();
		BoundingBox nodeCentroidBox = BoundingBox(
			(*this->shapePtrs[buildNode.getStartIndex()])->getCentroid(),
			(*this->shapePtrs[buildNode.getStartIndex()])->getCentroid());

		// Expand the current build node's box to surround all relevant children.
		for (int i = (buildNode.getStartIndex() + 1); i < buildNode.getEndIndex(); i++)
		{
			const Shape &selectedShape = *(*this->shapePtrs[i]);
			BoundingBox selectedShapeBox = selectedShape.getBoundingBox();
			nodeBox.expandToInclude(selectedShapeBox);
			nodeCentroidBox.expandToInclude(selectedShape.getCentroid());
		}

		// Assign the newly expanded bounding box to the selected flat node.
		flatNode.setBoundingBox(nodeBox);

		// If the number of shapes in this node is less than or equal to the "leaf capacity",
		// then this node will become a leaf. This is signified by its right offset of zero.
		if (flatNode.getNumPrimitives() <= this->leafCapacity)
		{
			flatNode.setRightOffset(BVH::LEAF_NODE_RIGHT_OFFSET);
			this->numLeaves++;
		}

		// Add the flat node to the flat tree.
		this->flatTree[flatTreeIndex] = flatNode;
		flatTreeIndex++;

		// If the child node touches a parent node, and the parent node is not the root node,
		// subtract one from the parent's right offset.
		if (buildNode.getParentIndex() != BVH::ROOT_PARENT_INDEX)
		{
			this->flatTree[buildNode.getParentIndex()].setRightOffset(
				this->flatTree[buildNode.getParentIndex()].getRightOffset() - 1);

			// If this is the second touch, the current node is the right child, and it will
			// set up the offset for the flattened tree.
			if (this->flatTree[buildNode.getParentIndex()].getRightOffset() == BVH::TOUCHED_TWICE)
			{
				this->flatTree[buildNode.getParentIndex()].setRightOffset(
					this->numNodes - 1 - buildNode.getParentIndex());
			}
		}

		// If the current node is a leaf, it is not subdivided.
		if (flatNode.getRightOffset() == BVH::LEAF_NODE_RIGHT_OFFSET)
		{
			continue;
		}

		// Otherwise, set up the split dimensions for partitioning on the longest axis.
		// Note: the split coordinate calculation depends on Vector3s being made up of
		// three doubles, in the order "x, y, z".
		Axis splitAxis = nodeCentroidBox.getLongestAxis();
		double splitCoordinate = 0.5 *
			(reinterpret_cast<const double*>(&nodeCentroidBox.getMin())[static_cast<int>(splitAxis)] +
			reinterpret_cast<const double*>(&nodeCentroidBox.getMax())[static_cast<int>(splitAxis)]);

		// Partition the array of objects on this particular split.
		int middle = buildNode.getStartIndex();
		for (int i = middle; i < buildNode.getEndIndex(); i++)
		{
			// Check the point coordinate of the selected shape with the split coordinate.
			double shapeSplitCoordinate = reinterpret_cast<const double*>(
				&(*this->shapePtrs[i])->getCentroid())[static_cast<int>(splitAxis)];
			if (shapeSplitCoordinate < splitCoordinate)
			{
				// Swap the selected shape with the middle shape.
				const Shape **temp = this->shapePtrs[i];
				this->shapePtrs[i] = this->shapePtrs[middle];
				this->shapePtrs[middle] = temp;
				middle++;
			}
		}

		// If a bad split occurs, then choose the center index.
		if ((middle == buildNode.getStartIndex()) || (middle == buildNode.getEndIndex()))
		{
			middle = buildNode.getStartIndex() +
				((buildNode.getEndIndex() - buildNode.getStartIndex()) / 2);
		}

		// Push the right and left child nodes onto the work stack.
		int start = buildNode.getStartIndex();
		int end = buildNode.getEndIndex();
		workArray[stackIndex] = BVHBuildEntry(middle, end, this->numNodes - 1);
		stackIndex++;
		workArray[stackIndex] = BVHBuildEntry(start, middle, this->numNodes - 1);
		stackIndex++;
	}
}

Intersection BVH::nearestHit(const Ray &ray) const
{
	// Intersection data, just like a naive "Ray::closestShape" implementation.
	double nearestT = Intersection::T_MAX;
	Vector3 nearestPoint;
	Vector3 nearestNormal;
	const Shape *nearestShape = nullptr;

	// T values for the current pairs of bounding box near/far hits.
	std::vector<double> boundingBoxHits = std::vector<double>(4);
	int closerNode;
	int otherNode;

	// The working set of traversal nodes.
	std::vector<BVHTraversal> workArray =
		std::vector<BVHTraversal>(BVH::MAX_BVH_TRAVERSAL_TO_DO);

	// Push the root node onto the working set. Be careful that the negative
	// intersection T max does not underflow.
	workArray[0] = BVHTraversal(0, -Intersection::T_MAX);

	// Begin processing node hits.
	int stackIndex = 0;
	while (stackIndex >= 0)
	{
		// Pop the next work node off the work array. This node holds the "nodeNumber"
		// and "nearT" values.
		BVHTraversal workNode = workArray[stackIndex];
		stackIndex--;

		// Obtain a pointer to the current node in the BVH flat tree.
		const BVHFlatNode &flatNode = this->flatTree[workNode.getIndex()];

		// If the current closest intersection is closer than this node, continue.
		if (nearestT < workNode.getMinT())
		{
			continue;
		}

		// If this node is a leaf node, try to intersect it with the ray, like any
		// other shape. This part is analogous to the "Ray::closestHit" method, only
		// now it's the BVH version.
		if (flatNode.getRightOffset() == BVH::LEAF_NODE_RIGHT_OFFSET)
		{
			for (int i = 0; i < flatNode.getNumPrimitives(); i++)
			{
				const Shape &selectedShape =
					*(*this->shapePtrs[flatNode.getStartIndex() + i]);

				Intersection currentTry = selectedShape.hit(ray);

				if (currentTry.getT() < nearestT)
				{
					// A closer intersection was found. Overwrite the most recent one.
					nearestT = currentTry.getT();
					nearestPoint = currentTry.getPoint();
					nearestNormal = currentTry.getNormal();
					nearestShape = currentTry.getShape();
				}
			}
		}

		// Otherwise, if this node is an internal node, check its children to see if
		// they will need to be pushed onto the work stack.
		else
		{
			// See if the ray intersects either of the children bounding boxes.
			bool hitChild0 = this->flatTree[workNode.getIndex() + 1]
				.getBoundingBox().intersects(ray, &boundingBoxHits[0], &boundingBoxHits[1]);
			bool hitChild1 = this->flatTree[workNode.getIndex() + flatNode.getRightOffset()]
				.getBoundingBox().intersects(ray, &boundingBoxHits[2], &boundingBoxHits[3]);

			// If both child nodes were hit, assume the left child is closer.
			if (hitChild0 && hitChild1)
			{
				closerNode = workNode.getIndex() + 1;
				otherNode = workNode.getIndex() + flatNode.getRightOffset();

				// Check if the right child actually was closer.
				if (boundingBoxHits[2] < boundingBoxHits[0])
				{
					// Swap the T and index values.
					double tempT = boundingBoxHits[0];
					boundingBoxHits[0] = boundingBoxHits[2];
					boundingBoxHits[2] = tempT;

					tempT = boundingBoxHits[1];
					boundingBoxHits[1] = boundingBoxHits[3];
					boundingBoxHits[3] = tempT;

					int tempIndex = closerNode;
					closerNode = otherNode;
					otherNode = tempIndex;
				}

				// It's possible that the nearest hit shape is still in the other node.

				// Push the farther node into the work stack first.
				stackIndex++;
				workArray[stackIndex] = BVHTraversal(otherNode, boundingBoxHits[2]);

				// And now push the closer node.
				stackIndex++;
				workArray[stackIndex] = BVHTraversal(closerNode, boundingBoxHits[0]);
			}

			// Else if only the left child was hit, push that one.
			else if (hitChild0)
			{
				stackIndex++;
				workArray[stackIndex] = BVHTraversal(workNode.getIndex() + 1, boundingBoxHits[0]);
			}

			// Else if only the right child was hit, push that one.
			else if (hitChild1)
			{
				stackIndex++;
				workArray[stackIndex] = BVHTraversal(
					workNode.getIndex() + flatNode.getRightOffset(), boundingBoxHits[2]);
			}
		}
	}

	// Set the intersection data from the ray attempting to intersect the flat tree in 
	// the intersection parameter.
	return Intersection(nearestT, nearestPoint, nearestNormal, nearestShape);
}