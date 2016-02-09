#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <string>

#include "../Utilities/Utility.h"

class Vector3
{
private:
	double x, y, z;
public:
	Vector3()
	{
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
	}

	Vector3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	static Vector3 randomColor()
	{
		return Vector3(
			Utility::rand0To1(),
			Utility::rand0To1(),
			Utility::rand0To1());
	}

	static Vector3 randomPointInSphere(const Vector3 &point, double radius)
	{
		Vector3 randPoint = Vector3(
			(2.0 * Utility::rand0To1()) - 1.0,
			(2.0 * Utility::rand0To1()) - 1.0,
			(2.0 * Utility::rand0To1()) - 1.0)
			.normalized().scaledBy(radius * Utility::fastRand0To1());
		return Vector3(
			point.x + randPoint.x,
			point.y + randPoint.y,
			point.z + randPoint.z);
	}

	static Vector3 randomPointInCuboid(const Vector3 &point, double width, double height,
		double depth)
	{
		return Vector3(
			point.x + (width * ((2.0 * Utility::rand0To1()) - 1.0)),
			point.y + (height * ((2.0 * Utility::rand0To1()) - 1.0)),
			point.z + (depth * ((2.0 * Utility::rand0To1()) - 1.0)));
	}

	static Vector3 randomDirectionInHemisphere(const Vector3 &normal)
	{
		Vector3 randDir = Vector3(
			(2.0 * Utility::fastRand0To1()) - 1.0,
			(2.0 * Utility::fastRand0To1()) - 1.0,
			(2.0 * Utility::fastRand0To1()) - 1.0).normalized();
		return (randDir.dot(normal) >= 0.0) ? randDir : (-randDir);
	}

	double getX() const { return this->x; }
	double getY() const { return this->y; }
	double getZ() const { return this->z; }

	Vector3 operator +(const Vector3 &v) const
	{
		return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
	}

	Vector3 operator -() const
	{
		return Vector3(-this->x, -this->y, -this->z);
	}

	Vector3 operator -(const Vector3 &v) const
	{
		return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
	}

	std::string toString() const
	{
		return std::string("[") +
			std::to_string(this->x) + std::string(", ") +
			std::to_string(this->y) + std::string(", ") +
			std::to_string(this->z) + std::string("]");
	}

	uint toRGB() const
	{
		return static_cast<uint>(
			((static_cast<uchar>(this->x * 255.0)) << 16) |
			((static_cast<uchar>(this->y * 255.0)) << 8) |
			((static_cast<uchar>(this->z * 255.0))));
	}

	double lengthSquared() const
	{
		return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
	}

	double length() const
	{
		return std::sqrt(this->lengthSquared());
	}

	Vector3 normalized() const
	{
		double lenRecip = 1.0 / this->length();
		return Vector3(
			this->x * lenRecip,
			this->y * lenRecip,
			this->z * lenRecip);
	}

	double dot(double m) const
	{
		return (this->x * m) +
			(this->y * m) +
			(this->z * m);
	}

	double dot(const Vector3 &v) const
	{
		return (this->x * v.x) +
			(this->y * v.y) +
			(this->z * v.z);
	}

	Vector3 cross(const Vector3 &v) const
	{
		return Vector3(
			(this->y * v.z) - (v.y * this->z),
			(v.x * this->z) - (this->x * v.z),
			(this->x * v.y) - (v.x * this->y));
	}

	Vector3 reflect(const Vector3 &normal) const
	{
		double vnDot = this->dot(normal);
		double vnSign = (vnDot > 0.0) ? 1.0 : ((vnDot < 0.0) ? -1.0 : 0.0);
		double vnDot2 = vnDot * 2.0;
		return Vector3(
			((vnSign * normal.x) * vnDot2) - this->x,
			((vnSign * normal.y) * vnDot2) - this->y,
			((vnSign * normal.z) * vnDot2) - this->z);
	}

	Vector3 scaledBy(double m) const
	{
		return Vector3(
			this->x * m,
			this->y * m,
			this->z * m);
	}

	Vector3 scaledBy(const Vector3 &v) const
	{
		return Vector3(
			this->x * v.x,
			this->y * v.y,
			this->z * v.z);
	}

	Vector3 lerp(const Vector3 &end, double percent) const
	{
		return Vector3(
			(this->x * (1.0 - percent)) + (end.x * percent),
			(this->y * (1.0 - percent)) + (end.y * percent),
			(this->z * (1.0 - percent)) + (end.z * percent));
	}

	Vector3 slerp(const Vector3 &end, double percent) const
	{
		double theta = std::acos(this->dot(end) / (this->length() * end.length()));
		double sinThetaRecip = 1.0 / std::sin(theta);
		Vector3 beginScaled = this->scaledBy(std::sin((1.0 - percent) * theta) * sinThetaRecip);
		Vector3 endScaled = end.scaledBy(std::sin(percent * theta) * sinThetaRecip);
		return beginScaled + endScaled;
	}

	Vector3 clamp() const
	{
		const double low = 0.0;
		const double high = 1.0;
		return Vector3(
			this->x > high ? high : (this->x < low ? low : this->x),
			this->y > high ? high : (this->y < low ? low : this->y),
			this->z > high ? high : (this->z < low ? low : this->z));
	}

	Vector3 componentMin(const Vector3 &v) const
	{
		return Vector3(
			(this->x < v.x) ? this->x : v.x,
			(this->y < v.y) ? this->y : v.y,
			(this->z < v.z) ? this->z : v.z);
	}

	Vector3 componentMax(const Vector3 &v) const
	{
		return Vector3(
			(this->x > v.x) ? this->x : v.x,
			(this->y > v.y) ? this->y : v.y,
			(this->z > v.z) ? this->z : v.z);
	}
};

#endif