#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <memory>

#include "../Math/Vector3.h"

class Intersection
{
private:
	double t;
	Vector3 point, normal;
	const class Shape *shape;
public:
	static const double T_MAX;

	Intersection();
	Intersection(double t, const Vector3 &point, const Vector3 &normal,
		const class Shape *shape);

	double getT() const;
	const Vector3 &getPoint() const;
	const Vector3 &getNormal() const;
	const class Shape *getShape() const;
};

#endif