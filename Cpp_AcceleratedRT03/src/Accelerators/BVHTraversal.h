#ifndef BVH_TRAVERSAL_H
#define BVH_TRAVERSAL_H

#include "../Utilities/Utility.h"

class BVHTraversal
{
private:
	int index;
	double minT;
public:
	BVHTraversal();
	BVHTraversal(int index, double minT);

	int getIndex() const;
	double getMinT() const;
};

#endif