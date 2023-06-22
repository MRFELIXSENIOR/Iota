#include "IotaBasic.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "SDL.h"

using namespace IotaEngine;

Util::Color::Color() {}
Util::Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}
SDL_Color Util::Color::data() {
	SDL_Color c = { r, g, b, a };
	return c;
}

Renderer::Renderer() {}
Renderer::Renderer(Window* win) {
	renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Application::ThrowRuntimeException(
			"Failed To Create Renderer!",
			Application::RuntimeException::RENDERER_CREATION_FAILURE, SDL_GetError());
	}
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer);
}

bool Renderer::Create(Window* win) {
	renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Application::ThrowRuntimeException(
			"Failed To Create Renderer!",
			Application::RuntimeException::RENDERER_CREATION_FAILURE, SDL_GetError());
		return false;
	}
	return true;
}

void Renderer::Start() {
	SDL_RenderClear(renderer);
}

void Renderer::End() {
	SDL_RenderPresent(renderer);
}

void Renderer::SetDrawColor(Util::Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Renderer::RenderTextureToScreen(Texture& texture) {
	SDL_RenderCopy(renderer, texture.data(), NULL, NULL);
}

Window::Window() {}
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
}

Window::~Window() {
	SDL_DestroyWindow(window);
}

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
	return true;
}

RenderSurface::RenderSurface() {}
RenderSurface::~RenderSurface() {}
