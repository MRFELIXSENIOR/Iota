#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"
#include "IotaSound.hpp"
#include "IotaTexture.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace iota;
using namespace Application;

static Window* app_window;

static bool app_running = false;
static bool app_initialized = false;

static int app_framelimit = 60;

bool Application::IsInitialized() { return app_initialized; }
bool Application::IsRunning() { return app_running; }

bool Application::Initialize(const std::string& window_title, int window_width, int window_height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ||
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0 ||
		TTF_Init() < 0)
		throw RuntimeError("SDL Initialization Failure, " + std::string(SDL_GetError()));

	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_MOD) < 0 || 
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) != 0 || 
		Mix_AllocateChannels(16) == -1)
		throw RuntimeError("SDL_mixer Initialization Failure, " + std::string(Mix_GetError()));

	Mix_GroupChannels(0, 15, SFX);
	Mix_GroupChannels(16, 21, AMBIENT);
	Mix_GroupChannels(22, 23, UI_SFX);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	app_window = new Window(window_title, window_width, window_height);
	SDL_RaiseWindow(app_window->GetDataPointer());

	app_initialized = true;
	return true;
}

bool Application::Exit() {
	if (!app_running)
		return false;

	std::cout << "Exiting...\n";
	app_running = false;

	Mix_CloseAudio();

	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();

	delete app_window;
	return true;
}

void Application::Start() {
	if (!app_initialized)
		throw RuntimeError("Application is not initialized");

	app_running = true;
	Basic::AppLoop();
}

void Application::SetFrameLimit(unsigned int target) { app_framelimit = target; }
int Application::GetFrameLimit() { return app_framelimit; }
