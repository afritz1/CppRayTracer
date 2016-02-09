#include "SphereLight.h"
#include "../Accelerators/BoundingBox.h"
#include "../Intersections/Intersection.h"
#include "../Materials/Flat.h"
#include "../Materials/Material.h"
#include "../Rays/Ray.h"

SphereLight::SphereLight(const Vector3 &point)
	: SphereLight(point, 0.5 + Utility::rand0To1(), Vector3::randomColor()) { }

SphereLight::SphereLight(const Vector3 &point, double radius, const Vector3 &color)
	: Light(), Sphere(point, radius, new Flat(color)) { }

const Vector3 &SphereLight::getCentroid() const
{
	return Sphere::getCentroid();
}

BoundingBox SphereLight::getBoundingBox() const
{
	return Sphere::getBoundingBox();
}

const Material &SphereLight::getMaterial() const
{
	return Sphere::getMaterial();
}

Vector3 SphereLight::randomPoint() const
{
	return Vector3::randomPointInSphere(this->getCentroid(), this->radius);
}


void SphereLight::moveTo(const Vector3 &point)
{
	Sphere::moveTo(point);
}

Intersection SphereLight::hit(const Ray &ray) const
{
	return Sphere::hit(ray);
}