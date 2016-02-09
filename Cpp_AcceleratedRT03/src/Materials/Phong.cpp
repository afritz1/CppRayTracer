#include <algorithm> // for std::min/max.

#include "Phong.h"
#include "../Intersections/Intersection.h"
#include "../Lights/Light.h"
#include "../Rays/Ray.h"
#include "../Utilities/Utility.h"
#include "../Worlds/World.h"

const double Phong::DEFAULT_AMBIENT = 0.60;
const double Phong::DEFAULT_SPECULAR = 0.30;
const double Phong::DEFAULT_SHINY = 16.0;
const double Phong::MAX_OCCLUSION_DISTANCE = 1.50;
int Phong::AMBIENT_SAMPLE_COUNT = 1;
int Phong::LIGHT_SAMPLE_COUNT = 1;

Phong::Phong(const Vector3 &color)
	: Phong(color, Phong::DEFAULT_AMBIENT, Phong::DEFAULT_SPECULAR,
	Phong::DEFAULT_SHINY) { }

Phong::Phong(const Vector3 &color, double ambient, double specular, double shiny)
	: Material(), color(color)
{
	this->ambient = ambient;
	this->specular = specular;
	this->shiny = shiny;
}

Phong Phong::randomPhong()
{
	return Phong(Vector3::randomColor(), Phong::DEFAULT_AMBIENT, 
		Phong::DEFAULT_SPECULAR, Phong::DEFAULT_SHINY);
}

int Phong::getAmbientSamples()
{
	return Phong::AMBIENT_SAMPLE_COUNT;
}

int Phong::getLightSamples()
{
	return Phong::LIGHT_SAMPLE_COUNT;
}

void Phong::incrementAmbientSamples()
{
	Phong::AMBIENT_SAMPLE_COUNT++;
}

void Phong::decrementAmbientSamples()
{
	Phong::AMBIENT_SAMPLE_COUNT = std::max(Phong::AMBIENT_SAMPLE_COUNT - 1, 1);
}

void Phong::incrementLightSamples()
{
	Phong::LIGHT_SAMPLE_COUNT++;
}

void Phong::decrementLightSamples()
{
	Phong::LIGHT_SAMPLE_COUNT = std::max(Phong::LIGHT_SAMPLE_COUNT - 1, 1);
}

Vector3 Phong::getBaseColor() const
{
	return this->color;
}

double Phong::getAmbientPercent(const Vector3 &point, const Vector3 &normal,
	const World &world) const
{
	const Vector3 pointNormalEps = point + normal.scaledBy(Utility::EPSILON);

	double percent = 0.0;
	for (int n = 0; n < Phong::AMBIENT_SAMPLE_COUNT; n++)
	{
		Vector3 hemisphereDir = Vector3::randomDirectionInHemisphere(normal);

		Ray hemisphereRay = Ray(pointNormalEps, hemisphereDir, Ray::INITIAL_DEPTH);
		Intersection currentTry = hemisphereRay.nearestHit(world);

		percent += (currentTry.getT() > Phong::MAX_OCCLUSION_DISTANCE) ?
			1.0 : (currentTry.getT() / Phong::MAX_OCCLUSION_DISTANCE);
	}

	return percent / static_cast<double>(Phong::AMBIENT_SAMPLE_COUNT);
}

Vector3 Phong::colorAt(const Intersection &intersection, const Ray &ray,
	const World &world) const
{
	Vector3 viewVector = -ray.getDirection();
	double vnDot = viewVector.dot(intersection.getNormal());
	double vnSign = vnDot > 0.0 ? 1.0 : ((vnDot < 0.0) ? -1.0 : 0.0);
	Vector3 localNormal = intersection.getNormal().scaledBy(vnSign);

	// Percent of the total ambient color visible at a point.
	double ambientPercent = this->getAmbientPercent(
		intersection.getPoint() + localNormal.scaledBy(Utility::EPSILON),
		localNormal, world);

	// Ambient component.
	Vector3 color = this->color.scaledBy(world.getBackgroundColor())
		.scaledBy(this->ambient * ambientPercent);

	// Diffuse component.
	for (const Light *light : world.getLights())
	{
		Vector3 totalDiffuseColor = Vector3();
		Vector3 totalHighlightColor = Vector3();

		int visibleSamples = 0;
		for (int n = 0; n < Phong::LIGHT_SAMPLE_COUNT; n++)
		{
			Vector3 lightDirection =
				(light->randomPoint() - intersection.getPoint()).normalized();

			Ray shadowRay = Ray(
				intersection.getPoint() + lightDirection.scaledBy(Utility::EPSILON),
				lightDirection,
				Ray::INITIAL_DEPTH);
			Intersection lightTry = light->hit(shadowRay);
			Intersection shadowTry = shadowRay.nearestShape(world);
			visibleSamples += (lightTry.getT() < shadowTry.getT());

			Vector3 lnReflect = lightDirection.reflect(localNormal).normalized();
			double lnDot = lightDirection.dot(localNormal);
			double lnReflectVDot = lnReflect.dot(viewVector);

			Vector3 highlightColor = light->getMaterial().getBaseColor().scaledBy(this->specular)
				.scaledBy(std::pow(std::max(0.0, lnReflectVDot), this->shiny));
			Vector3 diffuseColor = this->color.scaledBy(light->getMaterial().getBaseColor())
				.scaledBy(std::max(0.0, lnDot));

			totalDiffuseColor = totalDiffuseColor + diffuseColor;
			totalHighlightColor = totalHighlightColor +
				((lightDirection.dot(localNormal) >= 0.0) ? highlightColor : Vector3());
		}

		double lightContribution =
			static_cast<double>(visibleSamples) /
			static_cast<double>(Phong::LIGHT_SAMPLE_COUNT);

		totalDiffuseColor = totalDiffuseColor.scaledBy(
			1.0 / static_cast<double>(Phong::LIGHT_SAMPLE_COUNT));

		totalHighlightColor = totalHighlightColor.scaledBy(
			1.0 / static_cast<double>(Phong::LIGHT_SAMPLE_COUNT));

		color = color + (totalDiffuseColor + totalHighlightColor).scaledBy(lightContribution);
	}

	return color;
}