#include "Cuboid.h"
#include "../Accelerators/BoundingBox.h"
#include "../Intersections/Intersection.h"
#include "../Materials/Material.h"
#include "../Materials/Phong.h"
#include "../Rays/Ray.h"

Cuboid::Cuboid(const Vector3 &point)
	: Cuboid(point, 0.5 + Utility::rand0To1(), 0.5 + Utility::rand0To1(),
	0.5 + Utility::rand0To1(), new Phong(Phong::randomPhong())) { }

Cuboid::Cuboid(const Vector3 &point, double width, double height, double depth,
	Material *material)
	: Shape(), point(point)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->material = material;
}

Cuboid::~Cuboid()
{
	delete this->material;
}

const Vector3 &Cuboid::getCentroid() const
{
	return this->point;
}

BoundingBox Cuboid::getBoundingBox() const
{
	Vector3 halfDiagonal = Vector3(
		this->width * 0.5,
		this->height * 0.5,
		this->depth * 0.5);
	Vector3 minPoint = this->getCentroid() - halfDiagonal;
	Vector3 maxPoint = this->getCentroid() + halfDiagonal;
	return BoundingBox(minPoint, maxPoint);
}

const Material &Cuboid::getMaterial() const
{
	return *this->material;
}

void Cuboid::moveTo(const Vector3 &point)
{
	this->point = point;
}

Intersection Cuboid::hit(const Ray &ray) const
{
	double nMinX, nMinY, nMinZ, nMaxX, nMaxY, nMaxZ;
	double tMin, tMax;
	double tX1 = (-width + this->getCentroid().getX() - ray.getPoint().getX()) /
		ray.getDirection().getX();
	double tX2 = (width + this->getCentroid().getX() - ray.getPoint().getX()) /
		ray.getDirection().getX();

	if (tX1 < tX2)
	{
		tMin = tX1;
		tMax = tX2;
		nMinX = -width;
		nMinY = 0.0;
		nMinZ = 0.0;
		nMaxX = width;
		nMaxY = 0.0;
		nMaxZ = 0.0;
	}
	else
	{
		tMin = tX2;
		tMax = tX1;
		nMinX = width;
		nMinY = 0.0;
		nMinZ = 0.0;
		nMaxX = -width;
		nMaxY = 0.0;
		nMaxZ = 0.0;
	}

	if (tMin > tMax)
	{
		return Intersection();
	}

	double tY1 = (-height + this->getCentroid().getY() - ray.getPoint().getY()) /
		ray.getDirection().getY();
	double tY2 = (height + this->getCentroid().getY() - ray.getPoint().getY()) /
		ray.getDirection().getY();

	if (tY1 < tY2)
	{
		if (tY1 > tMin)
		{
			tMin = tY1;
			nMinX = 0.0;
			nMinY = -height;
			nMinZ = 0.0;
		}
		if (tY2 < tMax)
		{
			tMax = tY2;
			nMaxX = 0.0;
			nMaxY = height;
			nMaxZ = 0.0;
		}
	}
	else
	{
		if (tY2 > tMin)
		{
			tMin = tY2;
			nMinX = 0.0;
			nMinY = height;
			nMinZ = 0.0;
		}
		if (tY1 < tMax)
		{
			tMax = tY1;
			nMaxX = 0.0;
			nMaxY = -height;
			nMaxZ = 0.0;
		}
	}

	if (tMin > tMax)
	{
		return Intersection();
	}

	double tZ1 = (-depth + this->getCentroid().getZ() - ray.getPoint().getZ()) /
		ray.getDirection().getZ();
	double tZ2 = (depth + this->getCentroid().getZ() - ray.getPoint().getZ()) /
		ray.getDirection().getZ();

	if (tZ1 < tZ2)
	{
		if (tZ1 > tMin)
		{
			tMin = tZ1;
			nMinX = 0.0;
			nMinY = 0.0;
			nMinZ = -depth;
		}
		if (tZ2 < tMax)
		{
			tMax = tZ2;
			nMaxX = 0.0;
			nMaxY = 0.0;
			nMaxZ = depth;
		}
	}
	else
	{
		if (tZ2 > tMin)
		{
			tMin = tZ2;
			nMinX = 0.0;
			nMinY = 0.0;
			nMinZ = depth;
		}
		if (tZ1 < tMax)
		{
			tMax = tZ1;
			nMaxX = 0.0;
			nMaxY = 0.0;
			nMaxZ = -depth;
		}
	}

	if (tMin > tMax)
	{
		return Intersection();
	}

	if (tMin < 0.0)
	{
		tMin = tMax;
		nMinX = nMaxX;
		nMinY = nMaxY;
		nMinZ = nMaxZ;
	}

	if (tMin >= 0.0)
	{
		return Intersection(tMin, ray.pointAt(tMin),
			Vector3(nMinX, nMinY, nMinZ).normalized(), this);
	}
	else { return Intersection(); }
}