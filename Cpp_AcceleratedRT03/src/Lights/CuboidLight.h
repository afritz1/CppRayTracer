#ifndef CUBOID_LIGHT_H
#define CUBOID_LIGHT_H

#include "Light.h"
#include "../Shapes/Cuboid.h"

class CuboidLight : public Light, public Cuboid
{
public:
	CuboidLight(const Vector3 &point);
	CuboidLight(const Vector3 &point, double width, double height, double depth,
		const Vector3 &color);

	virtual const class Vector3 &getCentroid() const override;
	virtual class BoundingBox getBoundingBox() const override;
	virtual const class Material &getMaterial() const override;
	virtual class Vector3 randomPoint() const override;
	virtual void moveTo(const Vector3 &point) override;
	virtual class Intersection hit(const class Ray &ray) const override;
};

#endif