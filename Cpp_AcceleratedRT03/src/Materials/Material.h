#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Math/Vector3.h"

class Material
{
public:
	Material();
	virtual ~Material();

	virtual Vector3 getBaseColor() const = 0;
	virtual Vector3 colorAt(const class Intersection &intersection, const class Ray &ray, 
		const class World &world) const = 0;
};

#endif