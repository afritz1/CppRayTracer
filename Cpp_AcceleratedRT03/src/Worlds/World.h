#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>

#include "../Math/Vector3.h"

class World
{
private:
	class Accelerator *accelerator;
	std::vector<class Shape*> shapes;
	std::vector<class Light*> lights;
	class Shape *grabbedShape;
	Vector3 backgroundColor;
	double fogDensity;

	static const double DEFAULT_FOG_DENSITY;

	World(const Vector3 &backgroundColor, double fogDensity);

	void addShape(class Shape *shape);
	void addLight(class Light *light);
	void rebuildAccelerator();
public:
	~World();

	static World *makeWorld1();

	const Vector3 &getBackgroundColor() const;
	const std::vector<class Shape*> &getShapes() const;
	const std::vector<class Light*> &getLights() const;
	const class Accelerator *getAccelerator() const;
	void randomizeBackground();
	void grabShape(const class Camera &camera);
	void releaseShape();
	bool holdingShape() const;
	void updateGrabbedShape(const class Camera &camera);
	void calculateIntersections(const std::vector<Vector3> &imageRays,
		const class Camera &camera, std::vector<class Intersection> &intersections,
		int area) const;
	Vector3 colorAt(const class Ray &ray, const class Intersection &intersection) const;
};

#endif