#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

#include "Vector3.h"

class Quaternion
{
private:
	double x, y, z, w;

	double lengthSquared() const
	{
		return (this->x * this->x) +
			(this->y * this->y) +
			(this->z * this->z) +
			(this->w * this->w);
	}

	double length() const
	{
		return std::sqrt(this->lengthSquared());
	}
public:
	Quaternion(double x, double y, double z, double w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion(const Vector3 &v, double w)
		: Quaternion(v.getX(), v.getY(), v.getZ(), w) { }

	Quaternion operator *(const Quaternion &q) const
	{
		// Order of quaternion multiplication matters, like transformations.
		Vector3 lXyz = Vector3(this->x, this->y, this->z);
		Vector3 rXyz = Vector3(q.x, q.y, q.z);
		return Quaternion(
			lXyz.scaledBy(q.w) + rXyz.scaledBy(this->w) + lXyz.cross(rXyz),
			(this->w * q.w) - lXyz.dot(rXyz));
	}

	static Quaternion identity()
	{
		return Quaternion(0.0, 0.0, 0.0, 1.0);
	}

	static Quaternion fromAxisAngle(const Vector3 &axis, double w)
	{
		if (axis.lengthSquared() < Utility::EPSILON)
		{
			return Quaternion::identity();
		}

		double halfW = w * 0.5;
		Vector3 normAxis = axis.normalized();
		return Quaternion(
			normAxis.getX() * std::sin(halfW),
			normAxis.getY() * std::sin(halfW),
			normAxis.getZ() * std::sin(halfW),
			std::cos(halfW)).normalized();
	}

	double getX() const { return this->x; }
	double getY() const { return this->y; }
	double getZ() const { return this->z; }
	double getW() const { return this->w; }

	Quaternion normalized() const
	{
		double lenRecip = 1.0 / this->length();
		return Quaternion(
			this->x * lenRecip,
			this->y * lenRecip,
			this->z * lenRecip,
			this->w * lenRecip);
	}
};

#endif