#ifndef RAY_H
#define RAY_H

#include "../Math/Vector3.h"

class Ray
{
private:
	Vector3 point, direction;
	int depth;

public:
	static const int INITIAL_DEPTH = 0;

	Ray(const Vector3 &point, const Vector3 &direction, int depth);

	const Vector3 &getPoint() const;
	const Vector3 &getDirection() const;
	int getDepth() const;
	Vector3 pointAt(double t) const;
	class Intersection nearestHit(const class World &world) const;
	class Intersection nearestShape(const class World &world) const;
	class Intersection nearestLight(const class World &world) const;
};

#endif