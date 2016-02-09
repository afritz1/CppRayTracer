#ifndef PHONG_H
#define PHONG_H

#include "Material.h"

// Phong will have the ambient occlusion built in once the Ambient class works.

class Phong : public Material
{
protected:
	Vector3 color;
	double ambient, specular, shiny;

	static const double DEFAULT_AMBIENT;
	static const double DEFAULT_SPECULAR;
	static const double DEFAULT_SHINY;
	static const double MAX_OCCLUSION_DISTANCE;
	static int AMBIENT_SAMPLE_COUNT;
	static int LIGHT_SAMPLE_COUNT;

	double getAmbientPercent(const Vector3 &point, const Vector3 &normal,
		const class World &world) const;
public:
	Phong(const Vector3 &color);
	Phong(const Vector3 &color, double ambient, double specular, double shiny);

	static Phong randomPhong();
	static int getAmbientSamples();
	static int getLightSamples();
	static void incrementAmbientSamples();
	static void decrementAmbientSamples();
	static void incrementLightSamples();
	static void decrementLightSamples();

	virtual Vector3 getBaseColor() const override;
	virtual Vector3 colorAt(const class Intersection &intersection, const class Ray &ray,
		const class World &world) const override;
};

#endif