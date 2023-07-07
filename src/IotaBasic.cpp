#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"

#include "SDL.h"

using namespace IotaEngine;

static WindowManager window_manager;

Color::Color(): red(0), green(0), blue(0), alpha(0) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha() {}
SDL_Color Color::data() {
	SDL_Color c = { red, green, blue, alpha };
	return c;
}

SDL_Color IotaEngine::GetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	return { red, green, blue, alpha };
}

Renderer::Renderer() : renderer(nullptr) {}
Renderer::Renderer(Window& win) {
	if (!win.window) {
		Application::ThrowRuntimeException("Cannot Create Renderer with Uninitialized Window", Application::RuntimeException::RENDERER_CREATION_FAILURE);
	}

	renderer = SDL_CreateRenderer(win.window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Application::ThrowRuntimeException(
			"Failed To Create Renderer!",
			Application::RuntimeException::RENDERER_CREATION_FAILURE, SDL_GetError());
	}

	window_manager.AddRenderer(this);
}
Renderer::~Renderer() { Destroy(); }

bool Renderer::Create(Window& win) {
	if (!win.window) {
		Application::ThrowRuntimeException("Cannot Create Renderer with Uninitialized Window", Application::RuntimeException::RENDERER_CREATION_FAILURE);
		return false;
	}

	renderer = SDL_CreateRenderer(win.window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Application::ThrowRuntimeException(
			"Failed To Create Renderer!",
			Application::RuntimeException::RENDERER_CREATION_FAILURE, SDL_GetError());
		return false;
	}

	window_manager.AddRenderer(this);
	return true;
}

void Renderer::Start() { SDL_RenderClear(renderer); }
void Renderer::End() { SDL_RenderPresent(renderer); }

void Renderer::SetDrawColor(Color color) { SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha); }
void Renderer::RenderTextureToScreen(Texture& texture) { SDL_RenderCopy(renderer, texture.data(), NULL, NULL); }
void Renderer::Destroy() {
	window_manager.RemoveRenderer(this);
	SDL_DestroyRenderer(renderer);
}

Window::Window() : window(nullptr) {}
Window::Window(std::string_view window_title, int window_width,
	int window_height) {
	window = SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!window) {
		Application::ThrowRuntimeException("Failed To Create Window!",
			Application::RuntimeException::WINDOW_CREATION_FAILURE,
			SDL_GetError());
	}

	window_manager.AddWindow(this);
}
Window::~Window() { Destroy(); }

bool Window::Create(std::string_view window_title, int window_width,
	int window_height) {
	window = SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!window) {
		Application::ThrowRuntimeException("Failed To Create Window!",
			Application::RuntimeException::WINDOW_CREATION_FAILURE,
			SDL_GetError());
		return false;
	}

	window_manager.AddWindow(this);
	return true;
}

void Window::Destroy() {
	window_manager.RemoveWindow(this);
	SDL_DestroyWindow(window);
}

WindowManager::WindowManager() {}
WindowManager::~WindowManager() {}

void WindowManager::RemoveWindow(Window* win) { 
	auto it = std::find(windows.begin(), windows.end(), win);
	if (it != windows.end()) {
		windows.erase(it);
	}
}

void WindowManager::RemoveRenderer(Renderer* rdrer) {	
	auto it = std::find(renderers.begin(), renderers.end(), rdrer);
	if (it != renderers.end()) {
		renderers.erase(it);
	}
}

void WindowManager::AddWindow(Window* window) { windows.push_back(window); }
void WindowManager::AddRenderer(Renderer* rdrer) { renderers.push_back(rdrer); }

void WindowManager::Clean() {
	for (Window* w : windows) {
		w->Destroy();
	}

	for (Renderer* r : renderers) {
		r->Destroy();
	}
}

void Application::BasicClean() {
	window_manager.Clean();
}

void Application::Clean(Window& win, Renderer& rdrer) {
	rdrer.Destroy();
	win.Destroy();
}