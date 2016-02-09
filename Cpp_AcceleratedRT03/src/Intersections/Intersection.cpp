#include <limits>

#include "Intersection.h"
#include "../Shapes/Shape.h"

// Using float max just in case -T_MAX would underflow.
const double Intersection::T_MAX = std::numeric_limits<float>::max();

Intersection::Intersection()
	: Intersection(Intersection::T_MAX, Vector3(), Vector3(), nullptr) { }

Intersection::Intersection(double t, const Vector3 &point, const Vector3 &normal,
	const Shape *shape)
	: point(point), normal(normal), shape(shape)
{
	this->t = t;
}

double Intersection::getT() const
{
	return this->t;
}

const Vector3 &Intersection::getPoint() const
{
	return this->point;
}

const Vector3 &Intersection::getNormal() const
{
	return this->normal;
}

const Shape *Intersection::getShape() const
{
	return this->shape;
}