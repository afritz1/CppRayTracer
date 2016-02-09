#ifndef SHAPE_H
#define SHAPE_H

// Shape is going to act as an interface this time. Derived shapes and lights
// will be the the implementation.

class Shape
{
public:
	Shape();
	virtual ~Shape();

	virtual const class Vector3 &getCentroid() const = 0;
	virtual class BoundingBox getBoundingBox() const = 0;
	virtual const class Material &getMaterial() const = 0;
	virtual void moveTo(const Vector3 &point) = 0;
	virtual class Intersection hit(const class Ray &ray) const = 0;
};

#endif