#include "Ray.h"
#include "../Accelerators/Accelerator.h"
#include "../Intersections/Intersection.h"
#include "../Lights/Light.h"
#include "../Worlds/World.h"

Ray::Ray(const Vector3 &point, const Vector3 &direction, int depth)
{
	this->point = point;
	this->direction = direction;
	this->depth = depth;
}

const Vector3 &Ray::getPoint() const
{
	return this->point;
}

const Vector3 &Ray::getDirection() const
{
	return this->direction;
}

int Ray::getDepth() const
{
	return this->depth;
}

Vector3 Ray::pointAt(double t) const
{
	return this->point + this->direction.scaledBy(t);
}

Intersection Ray::nearestHit(const World &world) const
{
	// Get the closest shape.
	Intersection shapeHit = Ray::nearestShape(world);

	// Get the closest light.
	Intersection lightHit = Ray::nearestLight(world);

	// Get the closer of the two.
	return (shapeHit.getT() < lightHit.getT()) ? shapeHit : lightHit;
}

Intersection Ray::nearestShape(const World &world) const
{
	return world.getAccelerator()->nearestHit(*this);
}

Intersection Ray::nearestLight(const World &world) const
{
	Intersection nearestLight = Intersection();
	Intersection currentTry;

	for (const Light *light : world.getLights())
	{
		currentTry = light->hit(*this);

		if (currentTry.getT() < nearestLight.getT())
		{
			nearestLight = currentTry;
		}
	}

	return nearestLight;
}