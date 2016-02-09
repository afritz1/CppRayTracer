#include "World.h"
#include "../Accelerators/Accelerator.h"
#include "../Accelerators/BVH.h"
#include "../Cameras/Camera.h"
#include "../Intersections/Intersection.h"
#include "../Lights/CuboidLight.h"
#include "../Lights/Light.h"
#include "../Lights/SphereLight.h"
#include "../Materials/Material.h"
#include "../Math/Vector3.h"
#include "../Rays/Ray.h"
#include "../Shapes/Cuboid.h"
#include "../Shapes/Shape.h"
#include "../Shapes/Sphere.h"
#include "../Utilities/Utility.h"

const double World::DEFAULT_FOG_DENSITY = 0.025;

World::World(const Vector3 &backgroundColor, double fogDensity)
{
	this->backgroundColor = backgroundColor;
	this->fogDensity = fogDensity;

	this->shapes = std::vector<Shape*>();
	this->lights = std::vector<Light*>();
	this->accelerator = nullptr;
	this->grabbedShape = nullptr;
}

World::~World()
{
	for (Shape *shape : this->shapes)
	{
		delete shape;
	}

	for (Light *light : this->lights)
	{
		delete light;
	}

	delete this->accelerator;
}

World *World::makeWorld1()
{
	World *w = new World(Vector3::randomColor(), World::DEFAULT_FOG_DENSITY);

	double worldRadius = 12.0;

	const int SPHERE_COUNT = 10;
	const int CUBOID_COUNT = 10;
	for (int i = 0; i < SPHERE_COUNT; i++)
	{
		w->addShape(new Sphere(
			Vector3::randomPointInSphere(Vector3(), worldRadius)));
	}
	for (int i = 0; i < CUBOID_COUNT; i++)
	{
		w->addShape(new Cuboid(
			Vector3::randomPointInSphere(Vector3(), worldRadius)));
	}

	const int SPHERE_LIGHT_COUNT = 1;
	const int CUBOID_LIGHT_COUNT = 1;
	for (int i = 0; i < SPHERE_LIGHT_COUNT; i++)
	{
		w->addLight(new SphereLight(
			Vector3::randomPointInSphere(Vector3(), worldRadius)));
	}
	for (int i = 0; i < CUBOID_LIGHT_COUNT; i++)
	{
		w->addLight(new CuboidLight(
			Vector3::randomPointInSphere(Vector3(), worldRadius)));
	}

	w->rebuildAccelerator();

	return w;
}

const Vector3 &World::getBackgroundColor() const
{
	return this->backgroundColor;
}

const std::vector<Shape*> &World::getShapes() const
{
	return this->shapes;
}

const std::vector<Light*> &World::getLights() const
{
	return this->lights;
}

const Accelerator *World::getAccelerator() const
{
	return this->accelerator;
}

void World::addShape(Shape *shape)
{
	this->shapes.push_back(shape);
	this->rebuildAccelerator();
}

void World::addLight(Light *light)
{
	this->lights.push_back(light);
	this->rebuildAccelerator();
}

void World::randomizeBackground()
{
	this->backgroundColor = Vector3::randomColor();
}

void World::grabShape(const Camera &camera)
{
	if (this->holdingShape()) { return; }

	Intersection nearestHit =
		Ray(camera.getEye(), camera.getForward(), Ray::INITIAL_DEPTH).nearestHit(*this);

	if (nearestHit.getT() < camera.getGrabDistance())
	{
		this->grabbedShape = const_cast<Shape*>(nearestHit.getShape());
	}
}

void World::releaseShape()
{
	if (this->holdingShape())
	{
		this->grabbedShape = nullptr;
	}
}

bool World::holdingShape() const
{
	return this->grabbedShape != nullptr;
}

void World::updateGrabbedShape(const Camera &camera)
{
	if (!this->holdingShape()) { return; }

	this->grabbedShape->moveTo(camera.getEye() + camera.getForward().normalized()
		.scaledBy(camera.getHoldDistance()));
	this->rebuildAccelerator();
}

void World::rebuildAccelerator()
{
	if (this->accelerator != nullptr)
	{
		delete this->accelerator;
	}

	this->accelerator = new BVH(this->shapes);
}

void World::calculateIntersections(const std::vector<Vector3> &imageDirections,
	const Camera &camera, std::vector<Intersection> &intersections,
	int area) const
{
	const Vector3 eye = camera.getEye();

#pragma omp parallel for
	for (int i = 0; i < area; i++)
	{
		intersections[i] = Ray(eye, imageDirections[i], Ray::INITIAL_DEPTH).nearestHit(*this);
	}
}

Vector3 World::colorAt(const Ray &ray, const Intersection &intersection) const
{
	if (intersection.getT() < Intersection::T_MAX)
	{
		double distance = intersection.getT();
		double percent =
			(1.0 / std::exp((distance * this->fogDensity) * (distance * this->fogDensity)));
		Vector3 color = intersection.getShape()->getMaterial().colorAt(intersection, ray, *this);
		return color.scaledBy(percent) + this->backgroundColor.scaledBy(1.0 - percent);
	}
	else { return this->backgroundColor; }
}