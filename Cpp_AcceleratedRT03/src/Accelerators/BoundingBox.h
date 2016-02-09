#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "../Math/Vector3.h"
#include "../Utilities/Utility.h"

class BoundingBox
{
private:
	Vector3 min, max, extent;
public:
	BoundingBox(const Vector3 &min, const Vector3 &max);

	const Vector3 &getMin() const;
	const Vector3 &getMax() const;
	const Vector3 &getExtent() const;
	Vector3 getCentroid() const;
	Axis getLongestAxis() const;
	void expandToInclude(const Vector3 &point);
	void expandToInclude(const BoundingBox &boundingBox);
	bool intersects(const class Ray &ray, double *tNear, double *tFar) const;
};

#endif