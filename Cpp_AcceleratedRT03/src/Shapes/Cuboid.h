#ifndef CUBOID_H
#define CUBOID_H

#include "Shape.h"
#include "../Math/Vector3.h"

class Cuboid : public Shape
{
protected:
	Vector3 point;
	double width, height, depth;
	class Material *material;
public:
	Cuboid(const Vector3 &point);
	Cuboid(const Vector3 &point, double width, double height, double depth,
		class Material *material);
	~Cuboid();

	virtual const class Vector3 &getCentroid() const override;
	virtual class BoundingBox getBoundingBox() const override;
	virtual const class Material &getMaterial() const override;
	virtual void moveTo(const Vector3 &point) override;
	virtual class Intersection hit(const class Ray &ray) const override;
};

#endif