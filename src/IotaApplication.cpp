#include "IotaApplication.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>

#include "SDL.h"
#include "SDL_image.h"

using namespace iota;
using namespace Application;

static Window app_window;
static Renderer app_renderer;

static bool app_running = false;
static bool app_initialized = false;

static int app_framelimit = 60;

bool Application::IsInitialized() { return app_initialized; }
bool Application::IsRunning() { return app_running; }

bool Application::Initialize(const std::string& window_title, int window_width, int window_height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		Application::Panic("SDL Initialization Failure");
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG))) {
		Application::Panic("SDL_Image Initialization Failure");
		return false;
	}

	Lua::LoadSTD();
	app_window.Create(window_title, window_width, window_height);
	app_renderer.Create(app_window);
	app_initialized = true;

	return true;
}

bool Application::Exit() {
	if (app_running == false)
		return false;

	IMG_Quit();
	SDL_Quit();
	app_running = false;
	return true;
}

void Application::Start() {
	if (app_initialized == false) {
		Application::Panic("Application Is Not Initialized!");
		return;
	}

	app_running = true;
	Uint32 framestart;
	float frametime;
	Lua::RunAllScript();
	while (app_running == true) {
		framestart = SDL_GetTicks();

		Event::PollEvent();
		app_renderer.Start();

		app_renderer.RenderScreen();

		app_renderer.End();

		frametime = (SDL_GetTicks() - framestart) / 1000.0f;

		int delaytime = static_cast<int>((1.0f / app_framelimit) * 1000.0f - frametime);
		if (delaytime > 0) {
			SDL_Delay(delaytime);
		}
	}
}

Window& Application::GetWindow() { return app_window; }
Renderer& Application::GetRenderer() { return app_renderer; }

void Application::SetFrameLimit(unsigned int target) {
	app_framelimit = target;
}
