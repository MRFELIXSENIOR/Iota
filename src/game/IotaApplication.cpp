#include "game/IotaApplication.hpp"
#include "common/IotaBasic.hpp"
#include "common/IotaEvent.hpp"

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

static bool run_script = false;

bool Application::Initialize(std::string_view window_title, int window_width, int window_height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ThrowRuntimeException("Failed To Initialize",
			RuntimeException::INITIALIZATION_FAILURE,
			SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG))) {
		ThrowRuntimeException("Failed To Initialize Image Loader!",
			RuntimeException::INITIALIZATION_FAILURE,
			SDL_GetError());
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

void Application::Start(Lua::Script& main_script) {
	if (app_initialized == false) {
		ThrowRuntimeException("Application Not Initialized", Application::RuntimeException::NO_INIT_ERROR);
		return;
	}

	app_running = true;
	Uint32 framestart;
	float frametime;
	while (app_running == true) {
		framestart = SDL_GetTicks();

		app_renderer.Start();
		Event::PollEvent();

		if (!run_script) {
			main_script.Run();
			run_script = true;
		}

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

#define index(v) static_cast<int>(v)

void Application::ThrowRuntimeException(
	std::string_view error_title, RuntimeException error_code,
	std::string_view error_message) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n' << "[END RUNTIME EXCEPTION]\n";
	Clean();
	std::exit(index(error_code));
}

void Application::ThrowRuntimeException(
	std::string_view error_title,
	RuntimeException error_code) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << '\n' << "[END RUNTIME EXCEPTION]\n";
	Clean();
	std::exit(index(error_code));
}

void Application::ThrowRuntimeException(
	RuntimeException error_code) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << index(error_code)
		<< '\n' << "[END RUNTIME EXCEPTION]\n";
	Clean();
	std::exit(index(error_code));
}

void Application::ThrowException(std::string_view error_title,
	Exception error_code,
	std::string_view error_message) {
	std::cerr << "[Exception Thrown!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n' << "[End Exception]\n";
}

void Application::ThrowException(std::string_view error_title,
	Exception error_code) {
	std::cerr << "[Exception Thrown!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << '\n' << "[End Exception]\n";
}

void Application::ThrowException(Exception error_code) {
	std::cerr << "[Exception Thrown!]\nException Code: " << index(error_code) << '\n' << "[End Exception]\n";
}
