#ifndef FLAT_H
#define FLAT_H

#include "Material.h"

class Flat : public Material
{
private:
	Vector3 color;
public:
	Flat();
	Flat(const Vector3 &color);

	virtual Vector3 getBaseColor() const override;
	virtual Vector3 colorAt(const class Intersection &intersection, const class Ray &ray,
		const class World &world) const override;
};

#endif