#include "IotaApplication.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>

#include "SDL.h"
#include "SDL_image.h"

using namespace IotaEngine;
using namespace Application;

static Window app_window;
static Renderer app_renderer;

static bool app_running = false;
static bool app_initialized = false;

static int app_framelimit = 60;

void Application::test() {
	std::cout << "test\n";
}

bool Application::Initialize(std::string_view window_title, int window_width, int window_height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw Application::RuntimeException("SDL Initialization Failure");
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG))) {
		throw Application::RuntimeException("SDL_Image Initialization Failure");
		return false;
	}

	app_initialized = true;
	app_window.Create(window_title, window_width, window_height);
	app_renderer.Create(app_window);
	Lua::LoadSTD();

	return true;
}

bool Application::Clean() {
	if (app_running == false)
		return false;

	BasicClean();
	IMG_Quit();
	SDL_Quit();
	app_running = false;
	return true;
}

bool Application::IsRunning() { return app_running; }

void Application::Start() {
	if (app_initialized == false) {
		throw Application::RuntimeException("Application Not Initialized");
		return;
	}

	app_running = true;
	Uint32 framestart;
	float frametime;
	while (app_running == true) {
		framestart = SDL_GetTicks();

		app_renderer.Start();
		Event::PollEvent();

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

void Application::ErrorHandle(const std::exception& e) {
	std::cerr << "[IOTA] [Error Thrown!]\n" << "[Message]: " << e.what() << '\n';
}

void Application::SetFrameLimit(unsigned int target) {
	app_framelimit = target;
}
