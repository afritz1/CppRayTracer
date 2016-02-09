#ifndef SPHERE_LIGHT_H
#define SPHERE_LIGHT_H

#include "Light.h"
#include "../Shapes/Sphere.h"

class SphereLight : public Light, public Sphere
{
public:
	SphereLight(const Vector3 &point);
	SphereLight(const Vector3 &point, double radius, const Vector3 &color);

	virtual const class Vector3 &getCentroid() const override;
	virtual class BoundingBox getBoundingBox() const override;
	virtual const class Material &getMaterial() const override;
	virtual class Vector3 randomPoint() const override;
	virtual void moveTo(const Vector3 &point) override;
	virtual class Intersection hit(const class Ray &ray) const override;
};

#endif