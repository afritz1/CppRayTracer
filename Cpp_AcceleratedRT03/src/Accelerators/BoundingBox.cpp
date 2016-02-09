#include "BoundingBox.h"
#include "../Rays/Ray.h"

BoundingBox::BoundingBox(const Vector3 &min, const Vector3 &max)
{
	this->min = min;
	this->max = max;
	this->extent = max - min;
}

const Vector3 &BoundingBox::getMin() const
{
	return this->min;
}

const Vector3 &BoundingBox::getMax() const
{
	return this->max;
}

const Vector3 &BoundingBox::getExtent() const
{
	return this->extent;
}

Vector3 BoundingBox::getCentroid() const
{
	return this->min + this->extent.scaledBy(0.5);
}

Axis BoundingBox::getLongestAxis() const
{
	Axis axis = Axis::X;
	if (this->extent.getY() > this->extent.getX())
		axis = Axis::Y;
	if (this->extent.getZ() > this->extent.getY())
		axis = Axis::Z;
	return axis;
}

void BoundingBox::expandToInclude(const Vector3 &point)
{
	this->min = this->min.componentMin(point);
	this->max = this->max.componentMax(point);
	this->extent = max - min;
}

void BoundingBox::expandToInclude(const BoundingBox &boundingBox)
{
	this->min = this->min.componentMin(boundingBox.min);
	this->max = this->max.componentMax(boundingBox.max);
	this->extent = max - min;
}

bool BoundingBox::intersects(const Ray &ray, double *tNear, double *tFar) const
{
	Vector3 point = this->min + this->extent.scaledBy(0.5);
	double width = this->max.getX() - this->min.getX();
	double height = this->max.getY() - this->min.getY();
	double depth = this->max.getZ() - this->min.getZ();

	double nMinX, nMinY, nMinZ, nMaxX, nMaxY, nMaxZ;
	double tMin, tMax;
	double tX1 = (-width + point.getX() - ray.getPoint().getX()) / ray.getDirection().getX();
	double tX2 = (width + point.getX() - ray.getPoint().getX()) / ray.getDirection().getX();

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
		return false;
	}

	double tY1 = (-height + point.getY() - ray.getPoint().getY()) / ray.getDirection().getY();
	double tY2 = (height + point.getY() - ray.getPoint().getY()) / ray.getDirection().getY();

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
		return false;
	}

	double tZ1 = (-depth + point.getZ() - ray.getPoint().getZ()) / ray.getDirection().getZ();
	double tZ2 = (depth + point.getZ() - ray.getPoint().getZ()) / ray.getDirection().getZ();

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

	/* These two statements don't appear to do anything. Maybe they can be removed? */
	*tNear = tMin < tMax ? tMin : tMax;
	*tFar = tMax > *tNear ? tMax : *tNear;

	return tMax >= tMin;
}