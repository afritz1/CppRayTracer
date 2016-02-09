#include "Flat.h"
#include "../Intersections/Intersection.h"
#include "../Rays/Ray.h"
#include "../Worlds/World.h"

Flat::Flat()
	: Flat(Vector3::randomColor()) { }

Flat::Flat(const Vector3 &color)
	: Material(), color(color) { }

Vector3 Flat::getBaseColor() const
{
	return this->color;
}

Vector3 Flat::colorAt(const Intersection &intersection, const Ray &ray, 
	const World &world) const
{
	return this->color;
}