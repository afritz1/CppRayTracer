#include <ctime>
#include <iostream>
#include <SDL.h>

#include "Program.h"
#include "../Cameras/Camera.h"
#include "../Materials/Phong.h"
#include "../Math/Vector3.h"
#include "../Rendering/Renderer.h"
#include "../Worlds/World.h"

const int Program::DEFAULT_SCREEN_WIDTH = 960;
const int Program::DEFAULT_SCREEN_HEIGHT = 640;
const int Program::DEFAULT_PIXEL_SIZE = 6;
const short Program::DEFAULT_FPS = 60;
const std::string Program::DEFAULT_SCREEN_TITLE = "C++ Ray Tracing!";
const short Program::DEFAULT_SCREEN_BPP = 32;
const bool Program::DEFAULT_SCREEN_IS_DOUBLE_BUFFERED = false;
const bool Program::DEFAULT_SCREEN_IS_FULLSCREEN = false;
const bool Program::DEFAULT_SCREEN_IS_HARDWARE_SURFACE = false;
const bool Program::DEFAULT_SCREEN_IS_RESIZABLE = true;
const int Program::DEFAULT_SCREEN_FLAGS =
	(Program::DEFAULT_SCREEN_IS_DOUBLE_BUFFERED ? SDL_DOUBLEBUF : 0) |
	(Program::DEFAULT_SCREEN_IS_FULLSCREEN ? SDL_FULLSCREEN : 0) |
	(Program::DEFAULT_SCREEN_IS_HARDWARE_SURFACE ? SDL_HWSURFACE : SDL_SWSURFACE) |
	(Program::DEFAULT_SCREEN_IS_RESIZABLE ? SDL_RESIZABLE : 0);

Program::Program()
{
	SDL_Init(SDL_INIT_VIDEO);
	srand((uint)time(nullptr));

	SDL_Surface *screen = SDL_SetVideoMode(Program::DEFAULT_SCREEN_WIDTH,
		Program::DEFAULT_SCREEN_HEIGHT, Program::DEFAULT_SCREEN_BPP,
		Program::DEFAULT_SCREEN_FLAGS);

	// Initialize ray tracer objects.
	this->camera = std::unique_ptr<Camera>(new Camera(
		Camera::defaultCamera(12.0, this->getScreenAspect())));

	this->renderer = std::unique_ptr<Renderer>(new Renderer(
		screen->w, screen->h, Program::DEFAULT_PIXEL_SIZE));

	this->world = std::unique_ptr<World>(World::makeWorld1());

	// Update the screen title with all the render settings. This can only be done
	// after the renderer is initialized.
	this->updateScreenTitle();

	this->frameStart = 0;
	this->frameEnd = 0;
	this->running = true;
	this->doneRendering = false;

	std::cout << "C++ Distributed Ray Tracing! By Aaron Fritz." << "\n\n";
	std::cout << "WASD/Space/LShift to move around." << "\n";
	std::cout << "Mouse + click to look around." << "\n";
	std::cout << "Hold F to move faster." << "\n";
	std::cout << "E/Q to grab and release an object." << "\n";
	std::cout << "Mouse scroll to move grabbed object closer or farther." << "\n";
	std::cout << "T/G to zoom in and out." << "\n";
	std::cout << "R to reset camera zoom." << "\n";
	std::cout << "B to randomize background color." << "\n";
	std::cout << "N to randomize world." << "\n";
	std::cout << "Comma/Period to change resolution quality (pixel size)." << "\n";
	std::cout << "Left/Right brackets to change lighting and direct shadow quality." << "\n";
	std::cout << "Semicolon/Apostrophe to change indirect shadow quality (ambient occlusion)." << "\n";
}

Program::~Program()
{
	SDL_Quit();
}

void Program::loop()
{
	SDL_Event sdl_event;
	while (this->running)
	{
		this->frameStart = SDL_GetTicks();

		this->handleEvents(sdl_event);
		this->tick();
		this->render();

		this->frameEnd = SDL_GetTicks();

		this->delay();
	}
}

int Program::getDelay() const
{
	return (1000 / Program::DEFAULT_FPS);
}

double Program::getScreenAspect() const
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	return (double)screen->w / (double)screen->h;
}

void Program::resizeScreen(int width, int height)
{
	SDL_SetVideoMode(width, height, Program::DEFAULT_SCREEN_BPP,
		Program::DEFAULT_SCREEN_FLAGS);

	this->renderer = std::unique_ptr<Renderer>(new Renderer(
		width, height, this->renderer->getPixelSize()));

	this->camera->setAspectRatio(this->getScreenAspect());

	this->updateScreenTitle();
	this->doneRendering = false;
}

void Program::renameScreen(const std::string &title)
{
	SDL_WM_SetCaption(title.c_str(), nullptr);
}

void Program::updateScreenTitle()
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	std::string fullTitle = Program::DEFAULT_SCREEN_TITLE + std::string(" ") +
		std::string("Screen resolution: ") + std::to_string(screen->w) +
		std::string("x") + std::to_string(screen->h) + std::string(", ") +
		std::string("Pixel Size: ") + std::to_string(this->renderer->getPixelSize()) +
		std::string(", ") + std::string("Light samples: ") + 
		std::to_string(Phong::getLightSamples()) + std::string(", ") +
		std::string("Ambient samples: ") + std::to_string(Phong::getAmbientSamples());
	this->renameScreen(fullTitle);
}

void Program::handleEvents(SDL_Event &sdlEvent)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	uint *pixels = (uint*)screen->pixels;

	// Keyboard and mouse events.
	while (SDL_PollEvent(&sdlEvent))
	{
		bool quit =
			(sdlEvent.type == SDL_QUIT) ||
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_ESCAPE));
		bool resize =
			(sdlEvent.type == SDL_VIDEORESIZE);
		bool rollUp =
			(sdlEvent.type == SDL_MOUSEBUTTONDOWN) &&
			(sdlEvent.button.button == SDL_BUTTON_WHEELUP);
		bool rollDown =
			(sdlEvent.type == SDL_MOUSEBUTTONDOWN) &&
			(sdlEvent.button.button == SDL_BUTTON_WHEELDOWN);
		bool grabShape =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_e));
		bool releaseShape =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_q));
		bool randomizeBackground = 
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_b));
		bool randomizeWorld =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_n));
		bool increasePixelSize =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_PERIOD));
		bool decreasePixelSize =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_COMMA));
		bool increaseLightSamples =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_RIGHTBRACKET));
		bool decreaseLightSamples =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_LEFTBRACKET));
		bool increaseAmbientSamples =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_QUOTE));
		bool decreaseAmbientSamples =
			((sdlEvent.type == SDL_KEYDOWN) &&
			(sdlEvent.key.keysym.sym == SDLK_SEMICOLON));

		if (quit)
		{
			this->running = false;
		}
		if (resize)
		{
			this->resizeScreen(sdlEvent.resize.w, sdlEvent.resize.h);
		}
		if (rollUp)
		{
			if (world->holdingShape())
			{
				this->camera->setHoldDistance(
					this->camera->getHoldDistance() + Camera::DEFAULT_HOLD_INCREMENT);
			}
			this->doneRendering = false;
		}
		if (rollDown)
		{
			if (world->holdingShape())
			{
				this->camera->setHoldDistance(
					this->camera->getHoldDistance() - Camera::DEFAULT_HOLD_INCREMENT);
			}
			this->doneRendering = false;
		}
		if (grabShape)
		{
			this->world->grabShape(*this->camera);
			this->doneRendering = false;
		}
		if (releaseShape)
		{
			this->world->releaseShape();
			this->camera->setHoldDistance(Camera::DEFAULT_HOLD_DISTANCE);
			this->doneRendering = false;
		}
		if (randomizeBackground)
		{
			this->world->randomizeBackground();
			this->doneRendering = false;
		}
		if (randomizeWorld)
		{
			this->world = std::unique_ptr<World>(World::makeWorld1());
			this->doneRendering = false;
		}
		if (increasePixelSize)
		{
			this->renderer->incrementPixelSize();
			this->renderer->clearFrameBuffer(pixels);
			this->updateScreenTitle();
			this->doneRendering = false;
		}
		if (decreasePixelSize)
		{
			this->renderer->decrementPixelSize();
			this->renderer->clearFrameBuffer(pixels);
			this->updateScreenTitle();
			this->doneRendering = false;
		}
		if (increaseLightSamples)
		{
			Phong::incrementLightSamples();
			this->updateScreenTitle();
			this->doneRendering = false;
		}
		if (decreaseLightSamples)
		{
			Phong::decrementLightSamples();
			this->updateScreenTitle();
			this->doneRendering = false;
		}
		if (increaseAmbientSamples)
		{
			Phong::incrementAmbientSamples();
			this->updateScreenTitle();
			this->doneRendering = false;
		}
		if (decreaseAmbientSamples)
		{
			Phong::decrementAmbientSamples();
			this->updateScreenTitle();
			this->doneRendering = false;
		}
	}

	// Add new camera code using mouseDeltaX and Y.
	int dx, dy;
	uchar clicked = SDL_GetRelativeMouseState(&dx, &dy);
	uchar *keys = SDL_GetKeyState(nullptr);
	bool leftClick = (clicked & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
	bool rightClick = (clicked & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
	bool forward = keys[SDLK_w] != 0;
	bool backward = keys[SDLK_s] != 0;
	bool left = keys[SDLK_a] != 0;
	bool right = keys[SDLK_d] != 0;
	bool up = keys[SDLK_SPACE] != 0;
	bool down = keys[SDLK_LSHIFT] != 0;
	bool turning = ((dx != 0) || (dy != 0)) && (leftClick || rightClick);
	bool zoomIn = keys[SDLK_t] != 0;
	bool zoomOut = keys[SDLK_g] != 0;
	bool zooming = zoomIn || zoomOut;
	bool fast = keys[SDLK_f] != 0;
	bool resetCamera = keys[SDLK_r] != 0;
	
	bool any = turning | forward | backward | left | right |
		up | down | zooming | fast | resetCamera;
	
	if (any)
	{
		if (zooming)
		{
			double zoomDiff = 0.04;
			double newZoom = this->camera->getZoom() * 
				(1.0 + (zoomDiff * zoomIn) - (zoomDiff * zoomOut));
			this->camera->setZoom(newZoom);
		}

		if (resetCamera)
		{
			this->camera->setZoom(Camera::DEFAULT_ZOOM);
		}

		Vector3 dv = Vector3();
		if (forward) { dv = dv + this->camera->getForward(); }
		if (backward) { dv = dv - this->camera->getForward(); }
		if (right) { dv = dv + this->camera->getRight(); }
		if (left) { dv = dv - this->camera->getRight(); }
		if (up) { dv = dv + this->camera->getUp(); }
		if (down) { dv = dv - this->camera->getUp(); }

		this->camera->moveBy(dv.normalized().scaledBy(fast ? 20.0 : 1.0));

		if (turning)
		{
			this->camera->rotate(dx, -dy);
		}

		this->doneRendering = false;
	}
}

void Program::tick()
{
	this->world->updateGrabbedShape(*this->camera);
}

void Program::render()
{
	SDL_Surface *screen = SDL_GetVideoSurface();

	if (!this->doneRendering)
	{
		this->renderer->render(*this->world, *this->camera, static_cast<uint*>(screen->pixels));
		this->doneRendering = true;
	}

	SDL_Flip(screen);
}

void Program::delay()
{
	size_t frameTime = this->frameEnd - this->frameStart;
	if (frameTime < this->getDelay())
	{
		SDL_Delay(static_cast<uint>(this->getDelay() - frameTime));
	}
	else
	{
#ifdef DEBUG
		std::cout << "Slow frame (" << frameTime << " ms)." << "\n";
#endif
	}
}