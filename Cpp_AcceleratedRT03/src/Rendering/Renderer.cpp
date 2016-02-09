#include <algorithm>
#include <limits>

#include "Renderer.h"
#include "../Cameras/Camera.h"
#include "../Intersections/Intersection.h"
#include "../Math/Vector3.h"
#include "../Rays/Ray.h"
#include "../Worlds/World.h"

Renderer::Renderer(int width, int height, int pixelSize)
{
	this->width = width;
	this->height = height;
	this->pixelSize = pixelSize;

	int area = this->getRenderWidth() * this->getRenderHeight();
	this->imageDirections = std::vector<Vector3>(area);
	this->intersections = std::vector<Intersection>(area);
}

int Renderer::getRenderWidth() const
{
	return std::numeric_limits<int>::max() + static_cast<int>(
		((static_cast<double>(this->width) / static_cast<double>(this->pixelSize)) -
		std::numeric_limits<int>::max()));
}

int Renderer::getRenderHeight() const
{
	return std::numeric_limits<int>::max() + static_cast<int>(
		((static_cast<double>(this->height) / static_cast<double>(this->pixelSize)) -
		std::numeric_limits<int>::max()));
}

int Renderer::getPixelSize() const
{
	return this->pixelSize;
}

void Renderer::incrementPixelSize()
{
	this->pixelSize = std::min(this->pixelSize + 1, Renderer::MAX_PIXEL_SIZE);
	this->rebuildBuffers();
}

void Renderer::decrementPixelSize()
{
	this->pixelSize = std::max(this->pixelSize - 1, Renderer::MIN_PIXEL_SIZE);
	this->rebuildBuffers();
}

void Renderer::rebuildBuffers()
{
	int area = this->getRenderWidth() * this->getRenderHeight();
	this->imageDirections.resize(area);
	this->intersections.resize(area);
}

void Renderer::clearFrameBuffer(uint *dst)
{
	std::memset(dst, 0, sizeof(*dst) * this->width * this->height);
}

void Renderer::render(const World &world, const Camera &camera, uint *dst)
{
	const int renderWidth = this->getRenderWidth();
	const int renderHeight = this->getRenderHeight();
	const int area = renderWidth * renderHeight;

	camera.calculateImageRays(this->imageDirections, renderWidth, renderHeight);
	world.calculateIntersections(this->imageDirections, camera, this->intersections, area);

	const Vector3 eye = camera.getEye();

	if (this->pixelSize == 1)
	{
#pragma omp parallel for
		for (int i = 0; i < area; i++)
		{
			dst[i] = world.colorAt(Ray(eye, this->imageDirections[i], Ray::INITIAL_DEPTH), 
				this->intersections[i]).clamp().toRGB();
		}
	}
	else
	{
#pragma omp parallel for
		for (int j = 0; j < renderHeight; j++)
		{
			for (int i = 0; i < renderWidth; i++)
			{
				int renderIndex = i + (j * renderWidth);

				uint colorRGB = world.colorAt(
					Ray(eye, this->imageDirections[renderIndex], Ray::INITIAL_DEPTH),
					this->intersections[renderIndex]).clamp().toRGB();

				for (int y = 0; y < this->pixelSize; y++)
				{
					for (int x = 0; x < this->pixelSize; x++)
					{
						int index =
							std::min((x + (i * this->pixelSize)), this->width - 1) +
							std::min((y + (j * this->pixelSize)), this->height - 1) * this->width;
						dst[index] = colorRGB;
					}
				}
			}
		}
	}
}