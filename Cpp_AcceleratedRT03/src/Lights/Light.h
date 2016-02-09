#ifndef LIGHT_H
#define LIGHT_H

#include "../Shapes/Shape.h"

// Light can inherit from Shape if Shape does not have a "point" member, because
// the derived Light types will inherit from the derived shapes which much have
// some form of "point" member or "centroid", although a "centroid" is probably
// outside this ray tracer's scope.

class Light : public Shape
{
public:
	Light();
	virtual ~Light();

	virtual const class Vector3 &getCentroid() const = 0;
	virtual class BoundingBox getBoundingBox() const = 0;
	virtual const class Material &getMaterial() const = 0;
	virtual class Vector3 randomPoint() const = 0;
	virtual class Intersection hit(const class Ray &ray) const = 0;
};

#endif