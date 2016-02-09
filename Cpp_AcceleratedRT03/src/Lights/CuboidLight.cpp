#include "CuboidLight.h"
#include "../Accelerators/BoundingBox.h"
#include "../Intersections/Intersection.h"
#include "../Materials/Flat.h"
#include "../Materials/Material.h"
#include "../Rays/Ray.h"

CuboidLight::CuboidLight(const Vector3 &point)
	: CuboidLight(point, 0.5 + Utility::rand0To1(), 0.5 + Utility::rand0To1(),
	0.5 + Utility::rand0To1(), Vector3::randomColor()) { }

CuboidLight::CuboidLight(const Vector3 &point, double width, double height, double depth,
	const Vector3 &color)
	: Light(), Cuboid(point, width, height, depth, new Flat(color)) { }

const Vector3 &CuboidLight::getCentroid() const
{
	return Cuboid::getCentroid();
}

BoundingBox CuboidLight::getBoundingBox() const
{
	return Cuboid::getBoundingBox();
}

const Material &CuboidLight::getMaterial() const
{
	return Cuboid::getMaterial();
}

Vector3 CuboidLight::randomPoint() const
{
	return Vector3::randomPointInCuboid(this->getCentroid(), this->width, this->height,
		this->depth);
}

void CuboidLight::moveTo(const Vector3 &point)
{
	Cuboid::moveTo(point);
}

Intersection CuboidLight::hit(const Ray &ray) const
{
	return Cuboid::hit(ray);
}