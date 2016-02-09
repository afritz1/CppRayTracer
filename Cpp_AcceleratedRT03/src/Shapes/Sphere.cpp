#include "Sphere.h"
#include "../Accelerators/BoundingBox.h"
#include "../Intersections/Intersection.h"
#include "../Materials/Material.h"
#include "../Materials/Phong.h"
#include "../Rays/Ray.h"
#include "../Utilities/Utility.h"

Sphere::Sphere(const Vector3 &point)
	: Sphere(point, 0.5 + Utility::rand0To1(), new Phong(Phong::randomPhong())) { }

Sphere::Sphere(const Vector3 &point, double radius, Material *material)
	: Shape(), point(point)
{
	this->radius = radius;
	this->radiusRecip = radius > 0.0 ? (1.0 / radius) : 0.0;
	this->radiusSquared = radius > 0.0 ? (radius * radius) : 0.0;
	this->material = material;
}

Sphere::~Sphere()
{
	delete this->material;
}

double Sphere::getRadius() const
{
	return this->radius;
}

const Vector3 &Sphere::getCentroid() const
{
	return this->point;
}

BoundingBox Sphere::getBoundingBox() const
{
	Vector3 minPoint = Vector3(
		this->getCentroid().getX() - this->radius,
		this->getCentroid().getY() - this->radius,
		this->getCentroid().getZ() - this->radius);
	Vector3 maxPoint = Vector3(
		this->getCentroid().getX() + this->radius,
		this->getCentroid().getY() + this->radius,
		this->getCentroid().getZ() + this->radius);
	return BoundingBox(minPoint, maxPoint);
}

const Material &Sphere::getMaterial() const
{
	return *this->material;
}

void Sphere::moveTo(const Vector3 &point)
{
	this->point = point;
}

Intersection Sphere::hit(const Ray &ray) const
{
	Vector3 op = this->point - ray.getPoint();
	double b = op.dot(ray.getDirection());
	double determinant = (b * b) - op.dot(op) + this->radiusSquared;

	if (determinant < 0.0)
	{
		return Intersection();
	}
	else
	{
		determinant = sqrt(determinant);
		double t = ((b - determinant) > Utility::EPSILON) ? (b - determinant) :
			(((b + determinant) > Utility::EPSILON) ? (b + determinant) : Intersection::T_MAX);
		Vector3 point = ray.pointAt(t);
		Vector3 normal = (point - this->getCentroid()).scaledBy(this->radiusRecip);
		return Intersection(t, point, normal, this);
	}
}