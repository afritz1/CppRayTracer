#ifndef ACCELERATOR_H
#define ACCELERATOR_H

class Accelerator
{
public:
	Accelerator();
	virtual ~Accelerator();

	virtual class Intersection nearestHit(const class Ray &ray) const = 0;
};

#endif