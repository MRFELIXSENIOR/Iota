#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"
#include "IotaMono.hpp"
#include "IotaEvent.hpp"
#include "IotaTexture.hpp"

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

static Window app_window;
static Renderer app_renderer;

static bool app_running = false;
static bool app_initialized = false;

static int app_framelimit = 60;

bool Application::IsInitialized() { return app_initialized; }
bool Application::IsRunning() { return app_running; }

bool Application::Initialize(const std::string& window_title, int window_width, int window_height, int argc, char** argv) {
#define SOL_ALL_SAFETIES_ON 1
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "SDL Initialization Failure", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "SDL_Image Initialization Failure", IMG_GetError());
		return false;
	}

	if (TTF_Init() < 0) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "SDL_ttf Initialization Failure", TTF_GetError());
		return false;
	}

	if (Mix_Init(MIX_INIT_OGG) < 0) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "SDL_mixer Initialization Failure", Mix_GetError());
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	app_window.Create(window_title, window_width, window_height);
	app_renderer.Create(app_window);

	std::vector<std::string> input;
	for (int i = 1; i < argc; ++i) {
		input.emplace_back(argv[i]);
	}

	SEnv::Initialize(input);

	app_initialized = true;
	return true;
}

bool Application::Exit() {
	if (app_running == false)
		return false;

	IMG_Quit();
	SDL_Quit();
	SEnv::Clean();
	std::cout << "Exiting...\n";
	app_running = false;
	return true;
}

void Application::Start() {
	if (app_initialized == false) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "Application Is Not Initialized!");
		return;
	}

	app_running = true;
	Uint32 framestart;
	float frametime;

	SDL_RaiseWindow(app_window.data());

	ActorMap& actor_map = GetActorMap();
	for (auto& a : actor_map) {
		a.second->Load();
	}

	while (app_running == true) {
		framestart = SDL_GetTicks();

		Event::PollEvent();
		SDL_RenderClear(app_renderer.data());

		for (auto& a : actor_map) {
			a.second->Render();
		}

		SDL_SetRenderDrawColor(app_renderer.data(), 0, 0, 0, 0);
		SDL_RenderPresent(app_renderer.data());

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

Window& Application::GetWindow() { return app_window; }
Renderer& Application::GetRenderer() { return app_renderer; }

void Application::SetFrameLimit(unsigned int target) {
	app_framelimit = target;
}
