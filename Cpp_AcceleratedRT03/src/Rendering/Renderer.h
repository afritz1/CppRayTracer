#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "../Intersections/Intersection.h"
#include "../Utilities/Utility.h"

// Reconstruct the renderer whenever the screen resolution or pixel size changes.

class Renderer
{
private:
	// It's okay to recalculate all of the data every call. It's the sizes that 
	// should remain constant for the lifetime of the renderer so dynamic allocation
	// isn't done every frame.
	std::vector<class Vector3> imageDirections;
	std::vector<Intersection> intersections;
	int width, height, pixelSize;

	static const int MIN_PIXEL_SIZE = 1;
	static const int MAX_PIXEL_SIZE = 16;

	int getRenderWidth() const;
	int getRenderHeight() const;
	void rebuildBuffers();
public:
	Renderer(int width, int height, int pixelSize);

	int getPixelSize() const;
	void incrementPixelSize();
	void decrementPixelSize();
	void clearFrameBuffer(uint *dst);
	void render(const class World &world, const class Camera &camera, uint *dst);
};

#endif