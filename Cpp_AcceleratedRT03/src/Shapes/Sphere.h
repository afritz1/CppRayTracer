#ifndef SPHERE_H
#define SPHERE_H

#include <memory>

#include "Shape.h"
#include "../Math/Vector3.h"

class Sphere : public Shape
{
protected:
	Vector3 point;
	double radius, radiusRecip, radiusSquared;
	class Material *material;
public:
	Sphere(const Vector3 &point);
	Sphere(const Vector3 &point, double radius, class Material *material);
	~Sphere();

	double getRadius() const;
	virtual const class Vector3 &getCentroid() const override;
	virtual class BoundingBox getBoundingBox() const override;
	virtual const class Material &getMaterial() const override;
	virtual void moveTo(const Vector3 &point) override;
	virtual class Intersection hit(const class Ray &ray) const override;
};

#endif