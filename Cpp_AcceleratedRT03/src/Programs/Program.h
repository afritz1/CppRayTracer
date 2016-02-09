#ifndef PROGRAM_H
#define PROGRAM_H

#include <memory>
#include <string>

#include "../Cameras/Camera.h"
#include "../Rendering/Renderer.h"
#include "../Utilities/Utility.h"
#include "../Worlds/World.h"

class Program
{
private:
	// Default render constants.
	static const int DEFAULT_SCREEN_WIDTH;
	static const int DEFAULT_SCREEN_HEIGHT;
	static const int DEFAULT_PIXEL_SIZE;
	static const short DEFAULT_FPS;
	static const std::string DEFAULT_SCREEN_TITLE;
	static const short DEFAULT_SCREEN_BPP;
	static const bool DEFAULT_SCREEN_IS_DOUBLE_BUFFERED;
	static const bool DEFAULT_SCREEN_IS_FULLSCREEN;
	static const bool DEFAULT_SCREEN_IS_HARDWARE_SURFACE;
	static const bool DEFAULT_SCREEN_IS_RESIZABLE;
	static const int DEFAULT_SCREEN_FLAGS;

	// Ray tracer objects.
	std::unique_ptr<class Camera> camera;
	std::unique_ptr<class Renderer> renderer;
	std::unique_ptr<class World> world;

	// Render objects.
	bool running;
	bool doneRendering;

	// Timing objects.
	uint frameStart;
	uint frameEnd;

	int getDelay() const;
	double getScreenAspect() const;
	void resizeScreen(int width, int height);
	void renameScreen(const std::string &title);
	void updateScreenTitle();
	void handleEvents(union SDL_Event &sdlEvent);
	void tick();
	void render();
	void delay();
public:
	Program();
	~Program();

	void loop();
};

#endif