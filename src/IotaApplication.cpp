#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"
#include "IotaScript.hpp"
#include "IotaTexture.hpp"

#include "MonoEngine/Engine.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace iota;
using namespace Application;

static Window* app_window;

static bool app_running = false;
static bool app_initialized = false;

static int app_framelimit = 60;

bool Application::IsInitialized() { return app_initialized; }
bool Application::IsRunning() { return app_running; }

#define decl_str(v) std::string(v)

bool Application::Initialize(const std::string& window_title, int window_width, int window_height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw RuntimeError("SDL Initialization Failure" + std::string(SDL_GetError()));
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		throw RuntimeError("SDL_Image Initialization Failure" + std::string(IMG_GetError()));
		return false;
	}

	if (TTF_Init() < 0) {
		throw RuntimeError("SDL_ttf Initialization Failure" + std::string(TTF_GetError()));
		return false;
	}

	if (Mix_Init(MIX_INIT_OGG) < 0) {
		throw RuntimeError("SDL_mixer Initialization Failure" + std::string(Mix_GetError()));
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	app_window = new Window(window_title, window_width, window_height);

	Mono::Initialize();

	app_initialized = true;
	return true;
}

bool Application::Exit() {
	if (!app_running)
		return false;

	std::cout << "Exiting...\n";
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	Mono::Clean();

	delete app_window;
	app_running = false;
	return true;
}

void Application::Start() {
	if (!app_initialized) {
		throw RuntimeError("Application Is Not Initialized");
		return;
	}

	const auto& scripts = GetScripts();
	Mono::RunScript(scripts);

	app_running = true;
	Uint32 framestart;
	float frametime;

	Window& current_window = Window::GetCurrentFocusedWindow();
	SDL_RaiseWindow(current_window.GetDataPointer());

	ActorMap& actor_map = GetActorMap();
	for (auto& a : actor_map) {
		a.second->Load();
	}

	while (app_running == true) {
		framestart = SDL_GetTicks();

		Event::PollEvent();
		SDL_RenderClear(current_window.GetRendererPointer());

		for (auto& a : actor_map) {
			a.second->Render();
		}

		SDL_SetRenderDrawColor(current_window.GetRendererPointer(), 0, 0, 0, 0);
		SDL_RenderPresent(current_window.GetRendererPointer());

		frametime = (SDL_GetTicks() - framestart) / 1000.0f;

		int delaytime = static_cast<int>((1.0f / app_framelimit) * 1000.0f - frametime);
		if (delaytime > 0) {
			SDL_Delay(delaytime);
		}

		for (auto& a : actor_map) {
			a.second->Update();
		}
	}
}

void Application::SetFrameLimit(unsigned int target) {
	app_framelimit = target;
}
